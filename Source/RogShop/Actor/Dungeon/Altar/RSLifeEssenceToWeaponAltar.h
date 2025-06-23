// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSBaseAltar.h"
#include "RSLifeEssenceToWeaponAltar.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API ARSLifeEssenceToWeaponAltar : public ARSBaseAltar
{
	GENERATED_BODY()
	
public:
	ARSLifeEssenceToWeaponAltar();

	virtual void BeginPlay() override;

// 상호작용
public:
	virtual void Interact(ARSDunPlayerCharacter* Interactor) override;
};
