// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonWaitWidget.h"

#include "RSGameInstance.h"
#include "RSLevelSubsystem.h"
#include "RSTycoonGameModeBase.h"
#include "Components/Button.h"

void URSTycoonWaitWidget::NativeOnInitialized()
{
	Super::NativeConstruct();

	SaleStartButton->OnClicked.AddDynamic(this, &URSTycoonWaitWidget::OnClickSalesStartButton);
	ManagementButton->OnClicked.AddDynamic(this, &URSTycoonWaitWidget::OnClickManagementButton);
	OutButton->OnClicked.AddDynamic(this, &URSTycoonWaitWidget::OnClickOutButton);
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
