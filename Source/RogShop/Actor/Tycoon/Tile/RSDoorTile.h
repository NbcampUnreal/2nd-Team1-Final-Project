// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSBaseTile.h"
#include "RSDoorTile.generated.h"

class ARSTableTile;
struct FCookFoodData;
class ARSTycoonCustomerCharacter;

UCLASS()
class ROGSHOP_API ARSDoorTile : public ARSBaseTile
{
	GENERATED_BODY()

public:
	ARSDoorTile();
	
	TILE_KEY()
	
	ARSTycoonCustomerCharacter* SpawnCustomer(const FName& FoodData, ARSTableTile* Target);

	FVector GetSpawnPoint() const { return CustomerSpawnPoint->GetComponentLocation(); }

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> CustomerSpawnPoint;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARSTycoonCustomerCharacter> CustomerType;
};
