// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RSCanPickup.generated.h"

// This class does not need to be modified.
UINTERFACE()
class URSCanPickup : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ROGSHOP_API IRSCanPickup
{
	GENERATED_BODY()

public:
	virtual void Pickup(AActor* Actor) = 0;
	virtual AActor* Drop(FVector DropLocation) = 0;
	
	virtual AActor* GetPickupActor() = 0;
};
