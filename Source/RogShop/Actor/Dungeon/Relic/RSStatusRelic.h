// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSBaseRelic.h"
#include "CharacterStatus.h"
#include "RSStatusRelic.generated.h"

UCLASS(Blueprintable)
class ROGSHOP_API URSStatusRelic : public URSBaseRelic
{
	GENERATED_BODY()
	
public:
	virtual void ApplyEffect(ARSDunPlayerCharacter* OwnerCharacter) override;

	virtual void LoadEffect(ARSDunPlayerCharacter* OwnerCharacter) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Status", meta = (AllowPrivateAccess = "true"))
	ECharacterStatus TargetStatus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Status", meta = (AllowPrivateAccess = "true"))
	float Amount;
};
