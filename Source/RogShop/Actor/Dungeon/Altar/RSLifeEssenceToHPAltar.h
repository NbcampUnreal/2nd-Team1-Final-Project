// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSBaseAltar.h"
#include "RSLifeEssenceToHPAltar.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API ARSLifeEssenceToHPAltar : public ARSBaseAltar
{
	GENERATED_BODY()

public:
	ARSLifeEssenceToHPAltar();

	virtual void BeginPlay() override;

// 상호작용
public:
	virtual void Interact(ARSDunPlayerCharacter* Interactor) override;
};
