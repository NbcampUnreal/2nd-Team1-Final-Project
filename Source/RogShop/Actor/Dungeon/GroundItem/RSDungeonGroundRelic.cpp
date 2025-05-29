// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundItem/RSDungeonGroundRelic.h"
#include "RogShop/UtilDefine.h"
#include "RSBaseRelic.h"
#include "RSDunPlayerCharacter.h"
#include "RSRelicInventoryComponent.h"

void ARSDungeonGroundRelic::InitInteractableRelic(FName NewDataTableKey, UStaticMesh* NewMesh, const TSubclassOf<URSBaseRelic> NewRelicClass)
{
	if (!NewMesh)
	{
		RS_LOG_C("Invalid StaticMesh Used", FColor::Red);
	}
	if (!NewRelicClass)
	{
		RS_LOG_C("Invalid Blueprint Class Used", FColor::Red);
	}

	DataTableKey = NewDataTableKey;

	MeshComp->SetStaticMesh(NewMesh);
	RelicClass = NewRelicClass;

	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ARSDungeonGroundRelic::Interact(ARSDunPlayerCharacter* Interactor)
{
	if (!Interactor)
	{
		return;
	}

	FString ObjectString = DataTableKey.ToString() + TEXT("Object");
	FName ObjectName = FName(*ObjectString);

	URSBaseRelic* SpawnRelic = NewObject<URSBaseRelic>(Interactor, ObjectName, EObjectFlags::RF_Transient, RelicClass->StaticClass());

	URSRelicInventoryComponent* RSPlayerWeaponComponent = Interactor->GetRSRelicInventoryComponent();
	if (SpawnRelic && RSPlayerWeaponComponent)
	{
		RSPlayerWeaponComponent->AddRelic(DataTableKey);
		SpawnRelic->ApplyEffect(Interactor);

		Destroy();
	}
}
