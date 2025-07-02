// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSCanPickup.h"
#include "GameFramework/Character.h"
#include "SkeletalMergingLibrary.h"
#include "RSTycoonPlayerCharacter.generated.h"

class ARSBaseTile;
class USphereComponent;
struct FInputActionValue;

UCLASS()
class ROGSHOP_API ARSTycoonPlayerCharacter : public ACharacter, public IRSCanPickup
{
	GENERATED_BODY()

public:
	ARSTycoonPlayerCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Pickup(AActor* Actor) override;
	virtual AActor* Drop(FTransform DropTransform) override;
	virtual void ResetPickup() override;
	
	UFUNCTION(BlueprintPure)
	bool IsPickup() const { return PickupActor != nullptr; }

	UFUNCTION(BlueprintPure)
	bool NowCooking() const { return bCooking; }
	
	void SetCooking(bool Value) { bCooking = Value; }

	virtual AActor* GetPickupActor() override { return PickupActor; }
	

protected:
	virtual void BeginPlay() override;

private:
	void PlayInteractAnimation(ARSBaseTile* InteractTile);

	void OnMove(const FInputActionValue& Value);
	void OnInteract(const FInputActionValue& Value);

private:
	// 스켈레탈 메시 관련
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SkeletalMesh", meta = (AllowPrivateAccess = true))
	FSkeletalMeshMergeParams SkeletalMeshMergeParams;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> PickupLocation;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> InteractSphere;

	UPROPERTY()
	TObjectPtr<AActor> PickupActor;

	//애님 몽타주
	UPROPERTY(EditDefaultsOnly, Category="AnimMontage")
	TObjectPtr<UAnimMontage> TakeOrderMontage;

	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage")
	TObjectPtr<UAnimMontage> PickupFoodMontage;

	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage")
	TObjectPtr<UAnimMontage> DropFoodMontage;

	bool bCooking;
};
