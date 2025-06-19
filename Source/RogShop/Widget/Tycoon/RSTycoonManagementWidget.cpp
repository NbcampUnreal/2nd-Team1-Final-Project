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
}

void URSTycoonManagementWidget::OnClickExpandTile()
{
	ARSTileMap* TileMap = Cast<ARSTileMap>(UGameplayStatics::GetActorOfClass(GetWorld(), ARSTileMap::StaticClass()));
	check(TileMap)

	int32 Width = FMath::RoundToInt(WidthBox->GetValue());
	int32 Height = FMath::RoundToInt(HeightBox->GetValue());

	TileMap->ChangeTileSize(Width, Height);

	GetOwningPlayer<ARSTycoonPlayerController>()->SetCameraLocationToCenter();
}

void URSTycoonManagementWidget::OnClickWaitMode()
{
	GetGameInstance()->GetSubsystem<URSSaveGameSubsystem>()->OnSaveRequested.Broadcast();
	GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>()->StartWaitMode();
}

void URSTycoonManagementWidget::OnChangeGold(int32 Value)
{
	GoldText->SetText(FText::FromString(FString::FromInt(Value)));
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
