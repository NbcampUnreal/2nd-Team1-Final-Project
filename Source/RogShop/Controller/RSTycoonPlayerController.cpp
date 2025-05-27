// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "RogShop/Widget/Tycoon/RSTycoonManagementWidget.h"
#include "RogShop/Widget/Tycoon/RSTycoonSaleResultWidget.h"
#include "RogShop/Widget/Tycoon/RSTycoonSaleWidget.h"
#include "RogShop/Widget/Tycoon/RSTycoonWaitWidget.h"

void ARSTycoonPlayerController::AddMoney(int32 Value)
{
	Money += Value;
	SaleWidget->SetGold(Money);
}

void ARSTycoonPlayerController::AddCustomerCount(int32 Value)
{
	CustomerCount += Value;
	SaleWidget->SetCustomerCount(CustomerCount);
}

void ARSTycoonPlayerController::StartWaitMode()
{
	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
	ChangeMainWidget(WaitWidget);
}

void ARSTycoonPlayerController::StartSaleMode()
{
	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
	ChangeMainWidget(SaleWidget);
}

void ARSTycoonPlayerController::EndSaleMode()
{
	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
	ChangeMainWidget(SaleResultWidget);
}

void ARSTycoonPlayerController::StartManagementMode()
{
	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
	ChangeMainWidget(ManagementWidget);
}

void ARSTycoonPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(true);
	
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	check(LocalPlayer)

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem)

	check(IMC)
	Subsystem->AddMappingContext(IMC, 0);

	WaitWidget = CreateWidget<URSTycoonWaitWidget>(this, WaitWidgetType.Get());
	SaleWidget = CreateWidget<URSTycoonSaleWidget>(this, SaleWidgetType.Get());
	SaleResultWidget = CreateWidget<URSTycoonSaleResultWidget>(this, SaleResultWidgetType.Get());
	ManagementWidget = CreateWidget<URSTycoonManagementWidget>(this, ManagementWidgetType.Get());
	
	StartWaitMode();
}

void ARSTycoonPlayerController::ChangeMainWidget(UUserWidget* ActiveWidget)
{
	WaitWidget->RemoveFromParent();
	SaleWidget->RemoveFromParent();
	SaleResultWidget->RemoveFromParent();
	ManagementWidget->RemoveFromParent();

	ActiveWidget->AddToViewport();
}
