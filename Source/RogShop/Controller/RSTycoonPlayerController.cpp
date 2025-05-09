// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonPlayerController.h"

#include "EnhancedInputSubsystems.h"

void ARSTycoonPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	check(LocalPlayer)

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem)

	check(IMC)
	Subsystem->AddMappingContext(IMC, 0);
}
