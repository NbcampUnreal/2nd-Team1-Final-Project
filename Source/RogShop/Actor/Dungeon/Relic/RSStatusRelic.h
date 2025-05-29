// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSBaseRelic.h"
#include "RSStatusRelic.generated.h"

UENUM(BlueprintType)
enum class EStatus : uint8
{
	NONE,
	HP,
	MaxHP,
	MoveSpeed,
	AttackPower,
	AttackSpeed,
};

UCLASS(Blueprintable)
class ROGSHOP_API URSStatusRelic : public URSBaseRelic
{
	GENERATED_BODY()
	
public:
	virtual void ApplyEffect(ARSDunPlayerCharacter* OwnerCharacter) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Status", meta = (AllowPrivateAccess = "true"))
	EStatus TargetStatus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Status", meta = (AllowPrivateAccess = "true"))
	float Amount;
};
