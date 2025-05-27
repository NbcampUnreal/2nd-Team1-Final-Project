// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "RogShop/Widget/Tycoon/RSTycoonHUDWidget.h"
#include "RogShop/Widget/Tycoon/RSTycoonSaleResultWidget.h"
#include "RogShop/Widget/Tycoon/RSTycoonWaitWidget.h"

void ARSTycoonPlayerController::AddMoney(int32 Value)
{
	Money += Value;
	MainHUD->SetGold(Money);
}

void ARSTycoonPlayerController::AddCustomerCount(int32 Value)
{
	CustomerCount += Value;
	MainHUD->SetCustomerCount(CustomerCount);
}

void ARSTycoonPlayerController::StartWait()
{
	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
	
	MainHUD->RemoveFromParent();
	SaleResultWidget->RemoveFromParent();
	
	WaitWidget->AddToViewport();
}

void ARSTycoonPlayerController::StartSale()
{
	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
	
	WaitWidget->RemoveFromParent();
	SaleResultWidget->RemoveFromParent();
	
	MainHUD->AddToViewport();
}

void ARSTycoonPlayerController::EndSale()
{
	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
	
	MainHUD->RemoveFromParent();
	WaitWidget->RemoveFromParent();
	
	SaleResultWidget->AddToViewport();
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
	MainHUD = CreateWidget<URSTycoonHUDWidget>(this, MainHUDType.Get());
	SaleResultWidget = CreateWidget<URSTycoonSaleResultWidget>(this, SaleResultWidgetType.Get());
	
	StartWait();
}