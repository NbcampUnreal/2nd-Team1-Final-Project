// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSTycoonEvent.h"
#include "RSTycoonDoorTileLimit.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API URSTycoonDoorTileLimit : public URSTycoonEvent
{
	GENERATED_BODY()
	
public:
	URSTycoonDoorTileLimit();
	
	virtual bool Condition_Implementation(UWorld* World) override;
	virtual void Fail_Implementation(UWorld* World) override;
	virtual void Success_Implementation(UWorld* World) override;

};
