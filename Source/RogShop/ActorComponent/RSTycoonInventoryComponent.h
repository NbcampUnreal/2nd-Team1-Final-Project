// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSBaseInventoryComponent.h"
#include "RSTycoonInventoryComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROGSHOP_API URSTycoonInventoryComponent : public URSBaseInventoryComponent
{
	GENERATED_BODY()

public:
	URSTycoonInventoryComponent();

protected:
	virtual void BeginPlay() override;

};
