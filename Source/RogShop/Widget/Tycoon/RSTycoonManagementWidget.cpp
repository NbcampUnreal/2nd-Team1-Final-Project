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

// #include "RSTycoonBuyNPCWidget.h"

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

	// NPC 버튼 입력 세팅 (WBP의 OnClicked 라는 브로드캐스트를 받으면 밑에 함수를 실행)
	//if (BuyNPCWidget_Waiter)
	//{
	//	BuyNPCWidget_Waiter->OnClicked.AddDynamic(this, &URSTycoonManagementWidget::HandleWaiterClick);
	//}

	//if (BuyNPCWidget_Chef)
	//{
	//	BuyNPCWidget_Chef->OnClicked.AddDynamic(this, &URSTycoonManagementWidget::HandleChefClick);
	//}

	if (CreateNPCButton)
	{
		CreateNPCButton->OnClicked.AddDynamic(this, &URSTycoonManagementWidget::PlayBuyNPCParentBorderSlide);
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

void URSTycoonManagementWidget::PlayBuyTileParentBorderSlide()
{
	if (BuyTileParentBorderSlide)
	{
		if (bIsBuyTileParentBorderValid == false)
		{
			if (bIsBuyNPCParentBorderValid)
			{
				PlayAnimation(BuyNPCParentBorderClose);
				bIsBuyNPCParentBorderValid = false;
			}

			PlayAnimation(BuyTileParentBorderSlide);
			bIsBuyTileParentBorderValid = true;
		}
		else
		{
			PlayAnimation(BuyTileParentBorderClose);
			bIsBuyTileParentBorderValid = false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BuyTileParentBorderSlide Null"));
	}
}

void URSTycoonManagementWidget::PlayBuyNPCParentBorderSlide()
{
	if (BuyNPCParentBorderSlide)
	{
		if (bIsBuyNPCParentBorderValid == false)
		{
			if (bIsBuyTileParentBorderValid)
			{
				PlayAnimation(BuyTileParentBorderClose);
				bIsBuyTileParentBorderValid = false;
			}

			PlayAnimation(BuyNPCParentBorderSlide);
			bIsBuyNPCParentBorderValid = true;
		}
		else
		{
			PlayAnimation(BuyNPCParentBorderClose);
			bIsBuyNPCParentBorderValid = false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BuyNPCParentBorderSlide Null"));
	}
}

void URSTycoonManagementWidget::HandleWaiterClick()
{
	UE_LOG(LogTemp, Warning, TEXT("Waiter Clicked!"));
}

void URSTycoonManagementWidget::HandleChefClick()
{
	UE_LOG(LogTemp, Warning, TEXT("Chef Clicked!"));
}