// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSCanPickup.h"
#include "GameFramework/Character.h"
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
	virtual AActor* Drop(FVector DropLocation) override;
	virtual AActor* GetPickupActor() override { return PickupActor; }

private:
	void OnMove(const FInputActionValue& Value);
	void OnInteract(const FInputActionValue& Value);

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> InteractSphere;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> PickupLocation;

	UPROPERTY()
	TObjectPtr<AActor> PickupActor;
};
