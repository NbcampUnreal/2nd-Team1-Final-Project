// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDungeonGroundIngredient.h"
#include "RogShop/UtilDefine.h"
#include "RSDunPlayerCharacter.h"
#include "RSDungeonIngredientInventoryComponent.h"

ARSDungeonGroundIngredient::ARSDungeonGroundIngredient()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ARSDungeonGroundIngredient::Interact(ARSDunPlayerCharacter* Interactor)
{
	if (!Interactor)
	{
		return;
	}

	URSDungeonIngredientInventoryComponent* DungeonInventoryComp = Interactor->GetRSDungeonIngredientInventoryComponent();

	if (DungeonInventoryComp)
	{
		DungeonInventoryComp->AddItem(DataTableKey, Quantity);

		Destroy();
	}
}

void ARSDungeonGroundIngredient::SetQuantity(int32 NewQuantity)
{
	Quantity = NewQuantity;
}
