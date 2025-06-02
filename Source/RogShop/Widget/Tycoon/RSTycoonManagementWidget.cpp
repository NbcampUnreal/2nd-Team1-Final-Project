// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonManagementWidget.h"

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

	if (BuyNPCBorder)
	{
		BuyNPCBorder->SetRenderTranslation(FVector2D(0.f, 0.f));
	}

	if (CreateNPCButton)
	{
		CreateNPCButton->OnClicked.AddDynamic(this, &URSTycoonManagementWidget::BuyNPCBorderSlide);
	}

	// NPC 버튼 입력 세팅 (부모 WBP의 OnClick 라는 브로드캐스트를 받으면 밑에 함수를 실행)
	if (BuyNPCWidget_Waiter)
	{
		BuyNPCWidget_Waiter->OnClick.AddDynamic(this, &URSTycoonManagementWidget::HandleWaiterClick);
		BuyNPCWidget_Waiter->SetNPCName(FText::FromString(TEXT("Waiter")));
	}

	if (BuyNPCWidget_Chef)
	{
		BuyNPCWidget_Chef->OnClick.AddDynamic(this, &URSTycoonManagementWidget::HandleChefClick);
		BuyNPCWidget_Chef->SetNPCName(FText::FromString(TEXT("Chef")));
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

void URSTycoonManagementWidget::BuyTileBorderSlide()
{
	if (BuyTileBorderSlideAni)
	{
		if (bIsBuyTileBorderValid == false)
		{
			if (bIsBuyNPCBorderValid)
			{
				PlayAnimation(BuyNPCBorderCloseAni);
				bIsBuyNPCBorderValid = false;
			}

			PlayAnimation(BuyTileBorderSlideAni);
			bIsBuyTileBorderValid = true;
		}
		else
		{
			PlayAnimation(BuyTileBorderCloseAni);
			bIsBuyTileBorderValid = false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BuyTileBorderSlideAni Null"));
	}
}

void URSTycoonManagementWidget::BuyNPCBorderSlide()
{
	if (BuyNPCBorderSlideAni)
	{
		if (bIsBuyNPCBorderValid == false)
		{
			if (bIsBuyTileBorderValid)
			{
				PlayAnimation(BuyTileBorderCloseAni);
				bIsBuyTileBorderValid = false;
			}

			PlayAnimation(BuyNPCBorderSlideAni);
			bIsBuyNPCBorderValid = true;
		}
		else
		{
			PlayAnimation(BuyNPCBorderCloseAni);
			bIsBuyNPCBorderValid = false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BuyNPCParentBorderSlide Null"));
	}
}

void URSTycoonManagementWidget::HandleWaiterClick()
{
	OnWaiterClicked_BP();
}

void URSTycoonManagementWidget::HandleChefClick()
{
	OnChefClicked_BP();
}
