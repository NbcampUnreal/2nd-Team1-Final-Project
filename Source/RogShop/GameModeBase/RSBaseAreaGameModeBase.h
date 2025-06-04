// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RSBaseAreaGameModeBase.generated.h"

class ARSDungeonGroundWeapon;

UCLASS()
class ROGSHOP_API ARSBaseAreaGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    const TArray<FName>& GetSpawnedWeaponRowNames() const;
    void AddSpawnedWeaponRowName(const FName& RowName);

    // 스폰된 무기 저장
    void AddSpawnedWeapon(ARSDungeonGroundWeapon* WeaponActor);

    bool GetIsEquipWeapon() const;

protected:
    virtual void BeginPlay() override;

private:
    // 첫 무기 획득 후 남은 무기 모두 제거
    UFUNCTION()
    void ClearRemainingWeapons(ARSDungeonGroundWeapon* PickedUpWeapon);

    UPROPERTY()
    TArray<FName> SpawnedWeaponRowNames;

    UPROPERTY()
    TArray<TObjectPtr<ARSDungeonGroundWeapon>> SpawnedWeaponActors;

    // 던전 입장전 첫 번째 무기를 획득했는지의 여부 판별용
    UPROPERTY()
    bool bIsEquipWeapon = false;
};
