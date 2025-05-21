// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSBaseRelicEffect.h"
#include "RSPlayerStatRelicEffect.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API URSPlayerStatRelicEffect : public URSBaseRelicEffect
{
	GENERATED_BODY()
	
public:
	// �θ� Ŭ������ ����� ApplyEffect �Լ� �������̵�
	virtual void ApplyEffect(ARSDunPlayerCharacter* Player, ARSDunPlayerController* PC, FShopItemData& ItemData) override;
};
