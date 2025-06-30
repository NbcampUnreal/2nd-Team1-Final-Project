// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSBaseRelic.h"
#include "RSLightningRelic.generated.h"

class UNiagaraSystem;

UCLASS(Blueprintable)
class ROGSHOP_API URSLightningRelic : public URSBaseRelic
{
	GENERATED_BODY()
	
public:
	virtual void ApplyEffect(ARSDunPlayerCharacter* OwnerCharacter) override;

	virtual void LoadEffect(ARSDunPlayerCharacter* OwnerCharacter) override;
	
private:
	UFUNCTION()
	void BindFunc(ARSDunBaseCharacter* TargetCharacter);

	UFUNCTION()
	void EnemyHitEffect(ARSDunBaseCharacter* TargetCharacter);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Amount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 Persent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> EffectNiagara;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Status|OwnerCharacter", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<ARSDunBaseCharacter> PlayerCharacter;
};
