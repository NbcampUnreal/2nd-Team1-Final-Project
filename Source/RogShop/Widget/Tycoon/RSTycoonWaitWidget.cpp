// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonWaitWidget.h"

#include "RSGameInstance.h"
#include "RSLevelSubsystem.h"
#include "RSTycoonGameModeBase.h"
#include "RSTycoonPlayerController.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "RogShop/Actor/Tycoon/RSTileMap.h"
#include "RogShop/Actor/Tycoon/Tile/RSBaseTile.h"
#include "RogShop/Actor/Tycoon/Tile/RSCookingTile.h"
#include "RogShop/Actor/Tycoon/Tile/RSDoorTile.h"
#include "RogShop/Actor/Tycoon/Tile/RSTableTile.h"

void URSTycoonWaitWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SaleStartButton->OnClicked.AddDynamic(this, &URSTycoonWaitWidget::OnClickSalesStartButton);
	ManagementButton->OnClicked.AddDynamic(this, &URSTycoonWaitWidget::OnClickManagementButton);
	OutButton->OnClicked.AddDynamic(this, &URSTycoonWaitWidget::OnClickOutButton);

	
	ARSTycoonPlayerController* PlayerController = GetWorld()->GetFirstPlayerController<ARSTycoonPlayerController>();
	check(PlayerController)
	PlayerController->OnChangeGold.AddDynamic(this, &URSTycoonWaitWidget::OnChangeGold);
}

void URSTycoonWaitWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//운영시 문, 화구, 테이블 타일이 한개 이상씩은 있어야함
	ARSTileMap* TileMap = Cast<ARSTileMap>(UGameplayStatics::GetActorOfClass(GetWorld(), ARSTileMap::StaticClass()));
	check(TileMap)

	int32 DoorCount = 0, CookingCount = 0, TableCount = 0;
	for (auto& Tile : TileMap->GetTiles())
	{
		 if (Tile->IsA<ARSDoorTile>())
		 {
			 DoorCount++;
		 }

		if (Tile->IsA<ARSCookingTile>())
		{
			CookingCount++;
		}
		
		if (Tile->IsA<ARSTableTile>())
		{
			TableCount++;
		}
	}

	InDoorTileText->SetText(FText::FromString(FString::FromInt(DoorCount)));
	InCookingTileText->SetText(FText::FromString(FString::FromInt(CookingCount)));
	InTableTileText->SetText(FText::FromString(FString::FromInt(TableCount)));

	if (DoorCount < 1 || CookingCount < 1 || TableCount < 1)
	{
		TutorialBorder->SetVisibility(ESlateVisibility::Visible);
		SaleStartButton->SetIsEnabled(false);
	}
	else
	{
		TutorialBorder->SetVisibility(ESlateVisibility::Hidden);
		SaleStartButton->SetIsEnabled(true);
	}
}

void URSTycoonWaitWidget::OnClickSalesStartButton()
{
	GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>()->StartSaleMode();
}

void URSTycoonWaitWidget::OnClickManagementButton()
{
	GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>()->StartManagementMode();
}

void URSTycoonWaitWidget::OnClickOutButton()
{
	// 레벨 이동
	URSGameInstance* RSGameInstance = Cast<URSGameInstance>(GetWorld()->GetGameInstance());
	if (RSGameInstance)
	{
		URSLevelSubsystem* LevelSubsystem = RSGameInstance->GetSubsystem<URSLevelSubsystem>();

		if (LevelSubsystem)
		{
			LevelSubsystem->TravelToLevel(ERSLevelCategory::BaseArea);
		}
	}
}

void URSTycoonWaitWidget::OnChangeGold(int32 Value)
{
	GoldText->SetText(FText::FromString(FString::FromInt(Value)));
}
