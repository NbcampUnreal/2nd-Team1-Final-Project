// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundItem/RSDungeonGroundRelic.h"
#include "RogShop/UtilDefine.h"
#include "RSBaseRelic.h"
#include "RSDunPlayerCharacter.h"
#include "RSRelicInventoryComponent.h"

void ARSDungeonGroundRelic::Interact(ARSDunPlayerCharacter* Interactor)
{
	if (!Interactor)
	{
		return;
	}

	URSRelicInventoryComponent* RSPlayerWeaponComponent = Interactor->GetRSRelicInventoryComponent();
	if (RSPlayerWeaponComponent)
	{
		RSPlayerWeaponComponent->ApplyRelic(DataTableKey);
		
		Destroy();
	}
}
