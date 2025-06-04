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
	ARSWeaponSpawnPadActor();

protected:
	virtual void BeginPlay() override;

// 컴포넌트
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* PadMesh;

	void SpawnWeapons();
};
