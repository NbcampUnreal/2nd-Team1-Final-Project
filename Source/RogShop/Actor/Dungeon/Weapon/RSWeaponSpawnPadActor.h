// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSWeaponSpawnPadActor.generated.h"

UCLASS()
class ROGSHOP_API ARSWeaponSpawnPadActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARSWeaponSpawnPadActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PadMesh;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	UDataTable* WeaponDataTable;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	int32 NumberOfWeapons = 3;

	void SpawnWeapons();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
