// Fill out your copyright notice in the Description page of Project Settings.


#include "RSBaseAreaGameModeBase.h"
#include "RSDungeonGroundWeapon.h"
#include "RSDunPlayerController.h"

void ARSBaseAreaGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        if (ARSDunPlayerController* RSController = Cast<ARSDunPlayerController>(PC))
        {
            RSController->OnFirstWeaponPickUp.AddDynamic(this, &ARSBaseAreaGameModeBase::ClearRemainingWeapons);
        }
    }
}

void ARSBaseAreaGameModeBase::ClearRemainingWeapons(ARSDungeonGroundWeapon* PickedUpWeapon)
{
    for (ARSDungeonGroundWeapon* Weapon : SpawnedWeaponActors)
    {
        if (Weapon && Weapon != PickedUpWeapon)
        {
            Weapon->Destroy();
        }
    }

    SpawnedWeaponActors.Empty();
    SpawnedWeaponRowNames.Empty();

    bIsEquipWeapon = true;
}

const TArray<FName>& ARSBaseAreaGameModeBase::GetSpawnedWeaponRowNames() const
{
    return SpawnedWeaponRowNames;
}

void ARSBaseAreaGameModeBase::AddSpawnedWeaponRowName(const FName& RowName)
{
    SpawnedWeaponRowNames.Add(RowName);
}

void ARSBaseAreaGameModeBase::AddSpawnedWeapon(ARSDungeonGroundWeapon* WeaponActor)
{
	SpawnedWeaponActors.Add(WeaponActor);
}

bool ARSBaseAreaGameModeBase::GetIsEquipWeapon() const
{
    return bIsEquipWeapon;
}
