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
	// 부모 클래스에 선언된 ApplyEffect 함수 오버라이드
	virtual void ApplyEffect(ARSDunPlayerCharacter* Player, ARSDunPlayerController* PC, FShopItemData& ItemData) override;
};
