// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSBaseFood.generated.h"

UCLASS()
class ROGSHOP_API ARSBaseFood : public AActor
{
	GENERATED_BODY()

public:
	ARSBaseFood();

public:
	UPROPERTY(EditAnywhere)
	FString FoodName;

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> TakeLocation;
};
