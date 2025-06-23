// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSBaseRelic.h"
#include "CharacterStatus.h"
#include "RSKillStatusRelic.generated.h"

class ARSDunBaseCharacter;

// 몬스터 처치시 플레이어 캐릭터의 특정 스탯이 올라가는 동작을 수행합니다.
UCLASS(Blueprintable)
class ROGSHOP_API URSKillStatusRelic : public URSBaseRelic
{
	GENERATED_BODY()

public:
	virtual void ApplyEffect(ARSDunPlayerCharacter* OwnerCharacter);

	virtual void LoadEffect(ARSDunPlayerCharacter* OwnerCharacter);

private:
	// 해당 유물을 획득 했을 때에만 바인딩할 수 있도록 바인딩이 2중으로 이루어져있습니다.
	// 몬스터가 죽었을 때 호출될 함수가 몬스터가 스폰됐을 때 바인딩됩니다.

	// 적(몬스터, 보스)가 스폰될 때 바인딩될 함수를 연결하는 함수
	UFUNCTION()
	void BindFunc(ARSDunBaseCharacter* TargetCharacter);

	// 적이 죽었을 때 호출될 함수
	UFUNCTION()
	void KillEffectStatus(ARSDunBaseCharacter* TargetCharacter);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Status", meta = (AllowPrivateAccess = "true"))
	ECharacterStatus TargetStatus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Status", meta = (AllowPrivateAccess = "true"))
	float Amount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Status", meta = (AllowPrivateAccess = "true"))
	int32 Persent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Status|OwnerCharacter", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<ARSDunPlayerCharacter> PlayerCharacter;
};
