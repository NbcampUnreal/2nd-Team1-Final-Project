// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDungeonGroundWeapon.h"
#include "RogShop/UtilDefine.h"
#include "Components/BoxComponent.h"
#include "RSBaseWeapon.h"
#include "RSDunPlayerCharacter.h"
#include "RSPlayerWeaponComponent.h"
#include "RSDataSubsystem.h"
#include "ItemInfoData.h"

ARSDungeonGroundWeapon::ARSDungeonGroundWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARSDungeonGroundWeapon::Interact(ARSDunPlayerCharacter* Interactor)
{
	if (!Interactor)
	{
		return;
	}

	UGameInstance* CurGameInstance = Interactor->GetGameInstance();
	if (!CurGameInstance)
	{
		return;
	}

	URSDataSubsystem* DataSubsystem = CurGameInstance->GetSubsystem<URSDataSubsystem>();
	if (!DataSubsystem)
	{
		return;
	}

	UDataTable* WeaponDetailDataTable = DataSubsystem->WeaponDetail;
	if (!WeaponDetailDataTable)
	{
		return;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = Interactor;
	SpawnParameters.Instigator = Interactor;

	ARSBaseWeapon* SpawnWeapon = GetWorld()->SpawnActor<ARSBaseWeapon>(WeaponClass, SpawnParameters);

	URSPlayerWeaponComponent* RSPlayerWeaponComponent = Interactor->GetRSPlayerWeaponComponent();
	if (SpawnWeapon && RSPlayerWeaponComponent)
	{
		FDungeonWeaponData* WeaponData = WeaponDetailDataTable->FindRow<FDungeonWeaponData>(DataTableKey, TEXT("Get WeaponData"));

		SpawnWeapon->SetNormalAttackDatas(WeaponData->NormalAttackData);
		SpawnWeapon->SetStrongAttackDatas(WeaponData->StrongAttackData);

		SpawnWeapon->SetDataTableKey(DataTableKey);
		RSPlayerWeaponComponent->EquipWeaponToSlot(SpawnWeapon);

		Destroy();
	}
}

void ARSDungeonGroundWeapon::SetWeaponClass(const TSubclassOf<ARSDungeonItemBase> NewWeaponClass)
{
	WeaponClass = NewWeaponClass;
}

