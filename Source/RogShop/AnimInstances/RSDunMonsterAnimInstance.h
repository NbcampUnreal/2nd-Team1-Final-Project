// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RSDunMonsterAnimInstance.generated.h"

class ARSDunMonsterCharacter;

UCLASS()
class ROGSHOP_API URSDunMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
    URSDunMonsterAnimInstance();

public:
    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
    // ���� ĳ���� ���� (�ҽ����Ͽ��� ���� ĳ���� ĳ���ÿ�)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
    TObjectPtr<ARSDunMonsterCharacter> MonsterCharacter;

    // �ӵ� (�ȱ� �ִϸ��̼� �Ǵܿ�)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    float GroundSpeed;

    // ��ü �ӵ� ���� (�ʿ� �� ���)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    FVector Velocity;

    // ���� ���� ���� (�׾��� �� ������ �ִϸ��̼� ��ȯ��)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
    bool bIsDead;

    // �̵��ϰ� �ִ� �� ���� ���� (Idle<->Walk/Run �ִϸ��̼� ��ȯ��)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
    bool bShouldMove;
	
};
