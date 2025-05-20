// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDungeonItemBase.h"

// Sets default values
ARSDungeonItemBase::ARSDungeonItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARSDungeonItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARSDungeonItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

