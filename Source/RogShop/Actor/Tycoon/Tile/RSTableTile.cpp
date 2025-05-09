// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTableTile.h"


ARSTableTile::ARSTableTile()
{
	PrimaryActorTick.bCanEverTick = true;

	FoodLocation = CreateDefaultSubobject<USceneComponent>("FoodLocation");
	FoodLocation->SetupAttachment(RootComponent);
}

void ARSTableTile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARSTableTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ARSTableTile::GetFoodLocation() const
{
	return FoodLocation->GetComponentLocation();
}

