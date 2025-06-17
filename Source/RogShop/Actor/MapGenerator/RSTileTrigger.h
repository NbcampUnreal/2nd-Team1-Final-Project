// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "RSTileTrigger.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API ARSTileTrigger : public ATriggerBox
{
	GENERATED_BODY()

public:
	ARSTileTrigger();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

private:
	bool bMonsterSpawned = false;
	FIntPoint TileCoord;

	void InitTileCoord();
	
};
