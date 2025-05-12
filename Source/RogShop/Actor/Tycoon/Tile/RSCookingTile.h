// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSBaseTile.h"
#include "RSCookingTile.generated.h"

UCLASS()
class ROGSHOP_API ARSCookingTile : public ARSBaseTile
{
	GENERATED_BODY()

public:
	ARSCookingTile();

	virtual void Tick(float DeltaTime) override;

	virtual void Interact() override;
	
protected:
	virtual void BeginPlay() override;

public:
};
