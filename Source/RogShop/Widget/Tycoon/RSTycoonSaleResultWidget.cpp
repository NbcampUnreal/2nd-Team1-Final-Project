// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonSaleResultWidget.h"

#include "RSTycoonGameModeBase.h"
#include "RSTycoonPlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void URSTycoonSaleResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ARSTycoonPlayerController* Controller = GetWorld()->GetFirstPlayerController<ARSTycoonPlayerController>();
	check(Controller)
	
	CoinText->SetText(FText::FromString(FString::FromInt(Controller->GetMoney())));
	CustomerCountText->SetText(FText::FromString(FString::FromInt(Controller->GetCustomerCount())));

	SubmitButton->OnClicked.AddDynamic(this, &URSTycoonSaleResultWidget::ReturnWaitMode);
}

void URSTycoonSaleResultWidget::ReturnWaitMode()
{
	GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>()->StartWait();
}
