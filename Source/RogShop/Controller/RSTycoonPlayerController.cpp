// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "RogShop/Widget/Tycoon/TycoonOperationWidget.h"

void ARSTycoonPlayerController::AddMoney(int32 Value)
{
	Money += Value;

	MainHUD->SetGold(Money);
}

void ARSTycoonPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	check(LocalPlayer)

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem)

	check(IMC)
	Subsystem->AddMappingContext(IMC, 0);

	MainHUD = CreateWidget<UTycoonOperationWidget>(this, MainHUDType.Get());
	MainHUD->AddToViewport();
}
