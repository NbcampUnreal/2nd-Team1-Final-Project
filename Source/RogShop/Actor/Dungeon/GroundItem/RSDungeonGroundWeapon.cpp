// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDungeonGroundWeapon.h"
#include "RogShop/UtilDefine.h"
#include "Components/BoxComponent.h"
#include "RSBaseWeapon.h"
#include "RSDunPlayerCharacter.h"
#include "RSPlayerWeaponComponent.h"

ARSDungeonGroundWeapon::ARSDungeonGroundWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARSDungeonGroundWeapon::InitInteractableWeapon(FName NewDataTableKey, UStaticMesh* NewMesh, const TSubclassOf<ARSDungeonItemBase> NewWeaponClass)
{
	if (!NewMesh)
	{
		RS_LOG_C("Invalid StaticMesh Used", FColor::Red);
	}
	if (!NewWeaponClass)
	{
		RS_LOG_C("Invalid Blueprint Class Used", FColor::Red);
	}

	DataTableKey = NewDataTableKey;

	MeshComp->SetStaticMesh(NewMesh);
	WeaponClass = NewWeaponClass;

	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ARSDungeonGroundWeapon::Interact(ARSDunPlayerCharacter* Interactor)
{
	if (!Interactor)
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
		SpawnWeapon->SetDataTableKey(DataTableKey);
		RSPlayerWeaponComponent->EquipWeaponToSlot(SpawnWeapon);

		Destroy();
	}
}

