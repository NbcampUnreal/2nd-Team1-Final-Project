// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSDungeonGroundItem.h"
#include "RSDungeonGroundWeapon.generated.h"

class ARSDungeonItemBase;
class UBoxComponent;

UCLASS()
class ROGSHOP_API ARSDungeonGroundWeapon : public ARSDungeonGroundItem
{
	GENERATED_BODY()
	
public:	
	ARSDungeonGroundWeapon();

// 상호작용
public:
	virtual void Interact(ARSDunPlayerCharacter* Interactor) override;

public:
	void SetWeaponClass(const TSubclassOf<ARSDungeonItemBase> NewWeaponClass);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TargetClass", meta = (AllowPrivateAccess = true))
	TSubclassOf<ARSDungeonItemBase> WeaponClass;	// 상호작용시 플레이어에게 넘겨줄 무기 블루프린트 클래스
};
