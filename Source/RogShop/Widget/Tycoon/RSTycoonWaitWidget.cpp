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
#include "RogShop/Actor/Tycoon/Tile/RSIceBoxTile.h"
#include "RogShop/Actor/Tycoon/Tile/RSTableTile.h"

void URSTycoonWaitWidget::SetEnableSaleButton(bool Value)
{
	SaleStartButton->SetIsEnabled(Value);
}

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
