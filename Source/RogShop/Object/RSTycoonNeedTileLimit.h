// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSTycoonEvent.h"
#include "RSTycoonNeedTileLimit.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API URSTycoonNeedTileLimit : public URSTycoonEvent
{
	GENERATED_BODY()

public:
	URSTycoonNeedTileLimit();
	
	virtual bool Condition_Implementation(UWorld* World) override;
	virtual void Fail_Implementation(UWorld* World) override;
	virtual void Success_Implementation(UWorld* World) override;

private:
	int32 DoorCount = 0, CookingCount = 0, TableCount = 0, IceBoxCount = 0;
};
