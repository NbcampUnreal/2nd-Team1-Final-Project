// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSTycoonNPC.h"
#include "RSTycoonWaiter.generated.h"

UCLASS()
class ROGSHOP_API ARSTycoonWaiter : public ARSTycoonNPC
{
	GENERATED_BODY()

public:
	ARSTycoonWaiter();
	
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
};
