// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundItem/RSDungeonGroundRelic.h"
#include "RogShop/UtilDefine.h"
#include "RSBaseRelic.h"
#include "RSDunPlayerCharacter.h"
#include "RSRelicInventoryComponent.h"

void ARSDungeonGroundRelic::SetRelicClass(const TSubclassOf<URSBaseRelic> NewRelicClass)
{
	RelicClass = NewRelicClass;
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
