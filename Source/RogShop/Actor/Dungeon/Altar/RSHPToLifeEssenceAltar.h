// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSBaseAltar.h"
#include "RSHPToLifeEssenceAltar.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API ARSHPToLifeEssenceAltar : public ARSBaseAltar
{
	GENERATED_BODY()

public:
	ARSHPToLifeEssenceAltar();

// 상호작용
public:
	virtual void Interact(ARSDunPlayerCharacter* Interactor) override;
};
