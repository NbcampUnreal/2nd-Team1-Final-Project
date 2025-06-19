// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonEvent.h"

#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "RogShop/Widget/Tycoon/RSTycoonEventViewWidget.h"


bool URSTycoonEvent::Condition_Implementation(UWorld* World)
{
	return true;
}

void URSTycoonEvent::Fail_Implementation(UWorld* World)
{
}

void URSTycoonEvent::Success_Implementation(UWorld* World)
{
}

AActor* URSTycoonEvent::GetActorOfClass(UWorld* World, TSubclassOf<AActor> ActorClass)
{
	return UGameplayStatics::GetActorOfClass(World, ActorClass.Get());
}

TArray<AActor*> URSTycoonEvent::GetAllActorsOfClass(UWorld* World, TSubclassOf<AActor> ActorClass)
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(World, ActorClass.Get(), Actors);

	return Actors;
}

APlayerController* URSTycoonEvent::GetPlayerController(UWorld* World)
{
	return World->GetFirstPlayerController();
}

AGameModeBase* URSTycoonEvent::GetGameModeBase(UWorld* World)
{
	return World->GetAuthGameMode();
}
