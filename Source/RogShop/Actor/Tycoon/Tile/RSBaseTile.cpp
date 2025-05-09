// Fill out your copyright notice in the Description page of Project Settings.


#include "RSBaseTile.h"

#include "RogShop/UtilDefine.h"


ARSBaseTile::ARSBaseTile()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>("SceneComp");
	RootComponent = SceneComp;
	
	Plane = CreateDefaultSubobject<UStaticMeshComponent>("Plane");
	Plane->SetupAttachment(RootComponent);
}

FVector ARSBaseTile::GetTileSize()
{
	FVector Min, Max;
	Plane->GetLocalBounds(Min, Max);

	return (Max - Min) * Plane->GetRelativeScale3D();
}


void ARSBaseTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARSBaseTile::BeginPlay()
{
	Super::BeginPlay();
}
