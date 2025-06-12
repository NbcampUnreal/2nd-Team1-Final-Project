// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonWaitWidget.h"

#include "RSGameInstance.h"
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
	GetGameInstance<URSGameInstance>()->TravelToLevel(BaseAreaLevel);
}
