// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSBaseAltar.h"
#include "RSLifeEssenceToConversionAltar.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API ARSLifeEssenceToConversionAltar : public ARSBaseAltar
{
	GENERATED_BODY()
	
public:
	ARSLifeEssenceToConversionAltar();

	virtual void BeginPlay() override;

// 상호작용
public:
	virtual void Interact(ARSDunPlayerCharacter* Interactor) override;
};
