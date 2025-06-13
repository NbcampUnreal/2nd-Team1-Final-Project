// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonSaleResultWidget.h"

#include "RSSaveGameSubsystem.h"
#include "RSTycoonGameModeBase.h"
#include "RSTycoonPlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void URSTycoonSaleResultWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SubmitButton->OnClicked.AddDynamic(this, &URSTycoonSaleResultWidget::ReturnWaitMode);
}

void URSTycoonSaleResultWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	ARSTycoonPlayerController* Controller = GetWorld()->GetFirstPlayerController<ARSTycoonPlayerController>();
	check(Controller)
	
	CoinText->SetText(FText::FromString(FString::FromInt(Controller->GetGold())));
	CustomerCountText->SetText(FText::FromString(FString::FromInt(Controller->GetCustomerCount())));
}

void URSTycoonSaleResultWidget::ReturnWaitMode()
{
	GetGameInstance()->GetSubsystem<URSSaveGameSubsystem>()->OnSaveRequested.Broadcast();
	
	GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>()->StartWaitMode();
}
