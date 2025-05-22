// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonWaiter.h"


// Sets default values
ARSTycoonWaiter::ARSTycoonWaiter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARSTycoonWaiter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARSTycoonWaiter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}