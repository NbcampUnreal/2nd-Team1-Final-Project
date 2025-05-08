// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSDunBaseCharacter.h"
#include "RSDunPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

struct FInputActionValue;

UCLASS()
class ROGSHOP_API ARSDunPlayerCharacter : public ARSDunBaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARSDunPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void Dodge(const FInputActionValue& value);
	UFUNCTION()
	void Interaction(const FInputActionValue& value);
	UFUNCTION()
	void NormalAttack(const FInputActionValue& value);
	UFUNCTION()
	void StrongAttack(const FInputActionValue& value);
	UFUNCTION()
	void FirstWeaponSlot(const FInputActionValue& value);
	UFUNCTION()
	void SecondWeaponSlot(const FInputActionValue& value);

// 카메라 관련
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> CameraComp;
};
