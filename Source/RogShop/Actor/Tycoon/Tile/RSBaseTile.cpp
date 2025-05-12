// Fill out your copyright notice in the Description page of Project Settings.


#include "RSBaseTile.h"

#include "RogShop/UtilDefine.h"


ARSBaseTile::ARSBaseTile()
{
	
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>("SceneComp");
	RootComponent = SceneComp;

	Plane = CreateDefaultSubobject<UStaticMeshComponent>("Plane");
	Plane->SetupAttachment(RootComponent);

	//기본 메시 설정
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMeshAsset.Succeeded())
	{
		Plane->SetStaticMesh(CubeMeshAsset.Object);
		Plane->SetRelativeScale3D(FVector(5, 5, 0.1));
	}
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
