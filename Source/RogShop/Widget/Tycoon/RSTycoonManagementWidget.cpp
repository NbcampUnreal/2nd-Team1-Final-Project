// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonManagementWidget.h"

#include "RSTycoonBuyTileWidget.h"
#include "RSTycoonGameModeBase.h"
#include "RSTycoonPlayerController.h"
#include "Components/Button.h"
#include "Components/SpinBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/Border.h"

#include "Kismet/GameplayStatics.h"
#include "RogShop/Actor/Tycoon/RSTileMap.h"
#include "RogShop/Actor/Tycoon/Tile/RSBaseTile.h"

void URSTycoonManagementWidget::PlayBuyTileParentBorderSlideIn()
{
	if (SlideIn)
	{
		if (bIsBuyTileParentBorderValid == false)
		{
			PlayAnimation(SlideIn);

			// 이 창을 다시 닫으려면 해당값 false로 변환 후 닫는 애니메이션 및 닫는 함수 필요
			bIsBuyTileParentBorderValid = true;
		}
		else
		{
			// UE_LOG(LogTemp, Warning, TEXT("Exist BuyTileParentBorder !"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SlidIn Null"));
	}
}

void URSTycoonManagementWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
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

	//골드 세팅
	ARSTycoonPlayerController* Controller = GetWorld()->GetFirstPlayerController<ARSTycoonPlayerController>();
	check(Controller)
	
	GoldText->SetText(FText::FromString(FString::FromInt(Controller->GetGold())));
	
	ExpandTileButton->OnClicked.AddDynamic(this, &URSTycoonManagementWidget::OnClickExpandTile);
	ReturnBaseAreaButton->OnClicked.AddDynamic(this, &URSTycoonManagementWidget::OnClickWaitMode);

	// 보더 초기 위치 세팅
	if (BuyTileParentBorder)
	{
		BuyTileParentBorder->SetRenderTranslation(FVector2D(0.f, 0.f));
	}
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
	GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>()->StartWait();
	
	ARSTileMap* TileMap = Cast<ARSTileMap>(UGameplayStatics::GetActorOfClass(GetWorld(), ARSTileMap::StaticClass()));
	check(TileMap)
	TileMap->SaveTileMap();
}
