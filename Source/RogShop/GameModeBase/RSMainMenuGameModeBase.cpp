// Fill out your copyright notice in the Description page of Project Settings.


#include "RSMainMenuGameModeBase.h"
#include "RSMainMenuPlayerController.h"

ARSMainMenuGameModeBase::ARSMainMenuGameModeBase()
{
	PlayerControllerClass = ARSMainMenuPlayerController::StaticClass();
}
