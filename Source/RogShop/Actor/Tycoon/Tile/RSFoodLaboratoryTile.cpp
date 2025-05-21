// Fill out your copyright notice in the Description page of Project Settings.


#include "RSFoodLaboratoryTile.h"


// Sets default values
ARSFoodLaboratoryTile::ARSFoodLaboratoryTile()
{
	TileDisplayName = TEXT("요리 연구대");
}

void ARSFoodLaboratoryTile::Interact()
{
	Super::Interact();

	//요리 연구 UI를 띄워 연구를 할 수 있음
}
