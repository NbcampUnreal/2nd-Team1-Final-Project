// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSCanPickup.h"
#include "RSTycoonNPC.h"
#include "SkeletalMergingLibrary.h"
#include "RSTycoonWaiterCharacter.generated.h"

class ARSTableTile;

UCLASS()
class ROGSHOP_API ARSTycoonWaiterCharacter : public ARSTycoonNPC, public IRSCanPickup
{
	GENERATED_BODY()

public:
	ARSTycoonWaiterCharacter();

	virtual void Pickup(AActor* Actor) override;
	virtual AActor* Drop(FVector DropLocation) override;
	virtual AActor* GetPickupActor() override { return PickupActor; }
	
protected:
	virtual void InteractTarget(AActor* TargetActor) override;
	virtual void BeginPlay() override;
	
private:
	void InteractTable(ARSTableTile* Table);
	
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> PickupLocation;
	
	UPROPERTY()
	TObjectPtr<AActor> PickupActor;

	// 스켈레탈 메시 관련
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SkeletalMesh", meta = (AllowPrivateAccess = true))
	FSkeletalMeshMergeParams SkeletalMeshMergeParams;
};
