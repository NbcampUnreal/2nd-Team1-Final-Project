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

public:	
// 해당 엑터의 메시 세팅 및 상호작용에 필요한 변수 세팅
	void InitInteractableWeapon(FName NewDataTableKey, UStaticMesh* NewMesh, const TSubclassOf<ARSDungeonItemBase> NewWeaponClass);

// 상호작용
public:
	virtual void Interact(ARSDunPlayerCharacter* Interactor) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TSubclassOf<ARSDungeonItemBase> WeaponClass;	// 상호작용시 플레이어에게 넘겨줄 무기 블루프린트 클래스
};
