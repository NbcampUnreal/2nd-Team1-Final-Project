// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonWaitWidget.h"

#include "RSTycoonGameModeBase.h"
#include "Components/Button.h"

void URSTycoonWaitWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SaleStartButton->OnClicked.AddDynamic(this, &URSTycoonWaitWidget::OnClickSalesStartButton);
	ManagementButton->OnClicked.AddDynamic(this, &URSTycoonWaitWidget::OnClickManagementButton);
	OutButton->OnClicked.AddDynamic(this, &URSTycoonWaitWidget::OnClickOutButton);
}

void URSTycoonWaitWidget::OnClickSalesStartButton()
{
	GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>()->StartSale();
}

void URSTycoonWaitWidget::OnClickManagementButton()
{
	GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>()->StartManagement();
}

void URSTycoonWaitWidget::OnClickOutButton()
{
	
}
