// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSBaseTile.h"
#include "RSDoorTile.generated.h"

class ARSTycoonCustomerCharacter;

UCLASS()
class ROGSHOP_API ARSDoorTile : public ARSBaseTile
{
	GENERATED_BODY()

public:
	ARSDoorTile();

	void SpawnCustomer();
	
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> CustomerSpawnPoint;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARSTycoonCustomerCharacter> CustomerType;
};
