// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSBaseTile.generated.h"

UCLASS()
class ROGSHOP_API ARSBaseTile : public AActor
{
	GENERATED_BODY()

public:
	ARSBaseTile();

	virtual void Tick(float DeltaTime) override;
	
	virtual void Interact() {}
	
	UFUNCTION(CallInEditor, Category="Tile")
	FVector GetTileSize();
	
protected:
	virtual void BeginPlay() override;
	
public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> SceneComp;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> Plane;

	// FText TileName = FText::FromString(TEXT("Default"));
	FString TileName = "Default";
};
