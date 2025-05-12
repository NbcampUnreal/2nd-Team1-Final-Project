// Fill out your copyright notice in the Description page of Project Settings.


#include "RSFoodLaboratoryTile.h"


// Sets default values
ARSFoodLaboratoryTile::ARSFoodLaboratoryTile()
{
	TileName =  TEXT("요리 연구대");
}

void ARSFoodLaboratoryTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARSFoodLaboratoryTile::Interact()
{
	Super::Interact();

	//요리 연구 UI를 띄워 연구를 할 수 있음
}

void ARSFoodLaboratoryTile::BeginPlay()
{
	Super::BeginPlay();
	
}
