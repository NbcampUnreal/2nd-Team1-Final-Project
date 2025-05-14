// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RSInteractable.generated.h"

class ARSDunPlayerCharacter;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URSInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ROGSHOP_API IRSInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Interact(ARSDunPlayerCharacter* Interactor) = 0;
};
