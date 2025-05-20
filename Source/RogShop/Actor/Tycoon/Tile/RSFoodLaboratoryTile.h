// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSBaseTile.h"
#include "RSFoodLaboratoryTile.generated.h"

UCLASS()
class ROGSHOP_API ARSFoodLaboratoryTile : public ARSBaseTile
{
	GENERATED_BODY()

public:
	ARSFoodLaboratoryTile();

	TILE_KEY()
	
	virtual void Interact() override;
	
};
