// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSWeaponSpawnPadActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponPickUp);

UCLASS()
class ROGSHOP_API ARSWeaponSpawnPadActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ARSWeaponSpawnPadActor();

protected:
	virtual void BeginPlay() override;

private:
	void SpawnWeapons();

// 컴포넌트
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> SceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> MeshComp;
};
