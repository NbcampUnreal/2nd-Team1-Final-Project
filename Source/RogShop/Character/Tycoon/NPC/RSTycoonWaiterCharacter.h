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
	virtual AActor* Drop(FTransform DropTransform) override;
	virtual void StopAllAction() override;

	virtual AActor* GetPickupActor() override { return PickupActor; }
	UFUNCTION(BlueprintPure)
	bool IsPickup() const { return PickupActor != nullptr; }

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

	// 애님 몽타주
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> PickupFoodMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> DropFoodMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> TakeOrderMontage;
};
