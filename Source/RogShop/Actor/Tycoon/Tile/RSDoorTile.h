// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSBaseTile.h"
#include "RSDoorTile.generated.h"

class ARSTableTile;
class ARSTycoonCustomerCharacter;

UCLASS()
class ROGSHOP_API ARSDoorTile : public ARSBaseTile
{
	GENERATED_BODY()

public:
	ARSDoorTile();
	
	GENERATED_TILE()
	
	ARSTycoonCustomerCharacter* SpawnCustomer(const FName& FoodData, ARSTableTile* TargetTable);

	FVector GetSpawnPoint() const { return CustomerSpawnPoint->GetComponentLocation(); }

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> CustomerSpawnPoint;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARSTycoonCustomerCharacter> CustomerType;
};
