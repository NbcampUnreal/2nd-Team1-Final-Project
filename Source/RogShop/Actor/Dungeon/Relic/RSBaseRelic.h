// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RSBaseRelic.generated.h"

class ARSDunPlayerCharacter;

UCLASS(Abstract, NotBlueprintable)
class ROGSHOP_API URSBaseRelic : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	virtual void ApplyEffect(ARSDunPlayerCharacter* OwnerCharacter);

	UFUNCTION()
	virtual void LoadEffect(ARSDunPlayerCharacter* OwnerCharacter);
};
