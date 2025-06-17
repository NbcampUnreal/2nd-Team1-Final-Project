// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSCanPickup.h"
#include "GameFramework/Character.h"
#include "SkeletalMergingLibrary.h"
#include "RSTycoonPlayerCharacter.generated.h"

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
	virtual AActor* GetPickupActor() override { return PickupActor; }

protected:
	virtual void BeginPlay() override;

private:
	// 스켈레탈 메시 관련
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SkeletalMesh", meta = (AllowPrivateAccess = true))
	FSkeletalMeshMergeParams SkeletalMeshMergeParams;

	void OnMove(const FInputActionValue& Value);
	void OnInteract(const FInputActionValue& Value);

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> PickupLocation;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> InteractSphere;

	UPROPERTY()
	TObjectPtr<AActor> PickupActor;
};
