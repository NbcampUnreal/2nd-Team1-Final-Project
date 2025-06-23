// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonManagementWidget.h"

#include "RSSaveGameSubsystem.h"
#include "RSTycoonGameModeBase.h"
#include "RSTycoonPlayerController.h"

#include "Components/Button.h"
#include "Components/SpinBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/Border.h"

#include "Kismet/GameplayStatics.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/Actor/Tycoon/RSTileMap.h"
#include "RogShop/Actor/Tycoon/Tile/RSBaseTile.h"
#include "Tycoon/NPC/RSTycoonChefCharacter.h"
#include "Tycoon/NPC/RSTycoonWaiterCharacter.h"

void URSTycoonManagementWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//구매 가능한 Tile들 생성
	ARSTileMap* TileMap = Cast<ARSTileMap>(UGameplayStatics::GetActorOfClass(GetWorld(), ARSTileMap::StaticClass()));
	check(TileMap)

	for (auto& TileType : TileMap->GetTileTypes())
	{
		URSTycoonBuyTileWidget* BuyTile = CreateWidget<URSTycoonBuyTileWidget>(GetOwningPlayer(), BuyTileWidgetType);
		BuyTile->SetInfo(TileType->GetDefaultObject<ARSBaseTile>());

		UVerticalBoxSlot* BuyTileSlot = BuyTileParent->AddChildToVerticalBox(BuyTile);
		BuyTileSlot->SetPadding(FMargin(0, 10));
	}

	// 보더 초기 위치 세팅
	BuyTileParentBorder->SetRenderTranslation(FVector2D(0.f, 0.f));
	BuyNPCBorder->SetRenderTranslation(FVector2D(0.f, 0.f));

	ExpandTileButton->OnClicked.AddDynamic(this, &URSTycoonManagementWidget::OnClickExpandTile);
	ReturnBaseAreaButton->OnClicked.AddDynamic(this, &URSTycoonManagementWidget::OnClickWaitMode);
	CreateNPCButton->OnClicked.AddDynamic(this, &URSTycoonManagementWidget::OpenAndCloseNPCLayout);
	WidthBox->OnValueChanged.AddDynamic(this, &URSTycoonManagementWidget::OnChangeTileSizeBox);
	HeightBox->OnValueChanged.AddDynamic(this, &URSTycoonManagementWidget::OnChangeTileSizeBox);

	ARSTycoonPlayerController* PlayerController = GetWorld()->GetFirstPlayerController<ARSTycoonPlayerController>();
	check(PlayerController)
	PlayerController->OnChangeGold.AddDynamic(this, &URSTycoonManagementWidget::OnChangeGold);
}

void URSTycoonManagementWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//골드 세팅
	ARSTycoonPlayerController* Controller = GetWorld()->GetFirstPlayerController<ARSTycoonPlayerController>();
	check(Controller)
	OnChangeGold(Controller->GetGold());

	//현재 타일의 크기로 초기화
	ARSTileMap* TileMap = Cast<ARSTileMap>(UGameplayStatics::GetActorOfClass(GetWorld(), ARSTileMap::StaticClass()));
	check(TileMap)
	
	WidthBox->SetValue(TileMap->GetWidth());
	HeightBox->SetValue(TileMap->GetHeight());
}

void URSTycoonManagementWidget::OnClickExpandTile()
{
	ARSTileMap* TileMap = Cast<ARSTileMap>(UGameplayStatics::GetActorOfClass(GetWorld(), ARSTileMap::StaticClass()));
	check(TileMap)

	int32 Width = FMath::RoundToInt(WidthBox->GetValue());
	int32 Height = FMath::RoundToInt(HeightBox->GetValue());

	TileMap->ChangeTileSize(Width, Height, true);
	GetOwningPlayer<ARSTycoonPlayerController>()->SetCameraLocationToCenter();

	//드는 돈 다시 갱신하기 위해 추가
	OnChangeTileSizeBox(0);
}

void URSTycoonManagementWidget::OnClickWaitMode()
{
	CloseBuyNPCLayout();
	CloseBuyTileLayout();
	
	GetGameInstance()->GetSubsystem<URSSaveGameSubsystem>()->OnSaveRequested.Broadcast();
	GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>()->StartWaitMode();
}

void URSTycoonManagementWidget::OnChangeGold(int32 Value)
{
	GoldText->SetText(FText::FromString(FString::FromInt(Value)));
}

void URSTycoonManagementWidget::OnChangeTileSizeBox(float Value)
{
	int32 Width = FMath::RoundToInt(WidthBox->GetValue());
	int32 Height = FMath::RoundToInt(HeightBox->GetValue());

	ARSTileMap* TileMap = Cast<ARSTileMap>(UGameplayStatics::GetActorOfClass(GetWorld(), ARSTileMap::StaticClass()));
	check(TileMap)
	int32 NeedPrice = TileMap->GetNeedPrice(Width, Height);

	TilePriceText->SetText(FText::FromString(FString::Printf(TEXT("%d 원"), NeedPrice)));

	ARSTycoonPlayerController* PlayerController = GetWorld()->GetFirstPlayerController<ARSTycoonPlayerController>();
	check(PlayerController)
	
	if (PlayerController->GetGold() < NeedPrice)
	{
		CanTileChangeText->SetColorAndOpacity(FColor::Red);
		CanTileChangeText->SetText(FText::FromString(TEXT("돈이 부족합니다!")));
		
		ExpandTileButton->SetIsEnabled(false);
	}
	else
	{
		CanTileChangeText->SetColorAndOpacity(FColor::Cyan);
		CanTileChangeText->SetText(FText::FromString(TEXT("변경 가능합니다!")));
		
		ExpandTileButton->SetIsEnabled(true);
	}
}

void URSTycoonManagementWidget::OpenBuyTileLayout()
{
	CloseBuyNPCLayout();

	if (!bOpenBuyTileLayout)
	{
		bOpenBuyTileLayout = true;
		PlayAnimation(BuyTileBorderSlideAni);
	}
}

void URSTycoonManagementWidget::CloseBuyTileLayout()
{
	if (bOpenBuyTileLayout)
	{
		bOpenBuyTileLayout = false;
		PlayAnimation(BuyTileBorderCloseAni);
	}
}

void URSTycoonManagementWidget::OpenBuyNPCLayout()
{
	CloseBuyTileLayout();

	if (!bOpenBuyNPCLayout)
	{
		bOpenBuyNPCLayout = true;
		PlayAnimation(BuyNPCBorderSlideAni);
	}
}

void URSTycoonManagementWidget::CloseBuyNPCLayout()
{
	if (bOpenBuyNPCLayout)
	{
		bOpenBuyNPCLayout = false;
		PlayAnimation(BuyNPCBorderCloseAni);
	}
}

void URSTycoonManagementWidget::OpenAndCloseNPCLayout()
{
	if (bOpenBuyNPCLayout)
	{
		CloseBuyNPCLayout();
	}
	else
	{
		OpenBuyNPCLayout();
	}
}
