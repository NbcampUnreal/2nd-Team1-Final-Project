// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RSDunPlayerAnimInstance.generated.h"

class UCharacterMovementComponent;

UCLASS()
class ROGSHOP_API URSDunPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	URSDunPlayerAnimInstance();

public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	TObjectPtr<ACharacter> Character;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "References")
	TObjectPtr<UCharacterMovementComponent> CharacterMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Data")
	FVector Velocity;	// �ӵ� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Data")
	float GroundSpeed;	// �������� �̵����� �ӵ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Data")
	bool bShouldMove;	// �̵� ������ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement Data")
	bool bIsFalling;	// ���� ������ ����
};
