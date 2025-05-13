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
    // 몬스터 캐릭터 참조 (소스파일에서 몬스터 캐릭터 캐스팅용)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
    TObjectPtr<ARSDunMonsterCharacter> MonsterCharacter;

    // 속도 (걷기 애니메이션 판단용)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    float GroundSpeed;

    // 전체 속도 벡터 (필요 시 사용)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    FVector Velocity;

    // 죽음 상태 여부 (죽었을 때 나오는 애니메이션 전환용)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
    bool bIsDead;

    // 이동하고 있는 지 상태 여부 (Idle<->Walk/Run 애니메이션 전환용)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
    bool bShouldMove;
	
};
