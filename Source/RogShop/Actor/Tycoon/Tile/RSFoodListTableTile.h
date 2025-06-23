// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSBaseTile.h"
#include "RSFoodListTableTile.generated.h"

UCLASS()
class ROGSHOP_API ARSFoodListTableTile : public ARSBaseTile
{
	GENERATED_BODY()

public:
	ARSFoodListTableTile();

	GENERATED_TILE()
	
	virtual void Interact(ACharacter* InteractCharacter) override;

private:
	bool bUIOpen;
};
