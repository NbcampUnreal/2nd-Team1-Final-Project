// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDungeonGroundIngredient.h"
#include "RogShop/UtilDefine.h"
#include "RSDunPlayerCharacter.h"
#include "RSDungeonIngredientInventoryComponent.h"

ARSDungeonGroundIngredient::ARSDungeonGroundIngredient()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ARSDungeonGroundIngredient::InitItemInfo(FName NewDataTableKey, UStaticMesh* NewMesh, int32 NewQuantity)
{
	if (!NewMesh)
	{
		RS_LOG_C("Invalid StaticMesh Used", FColor::Red);
	}

	DataTableKey = NewDataTableKey;

	MeshComp->SetStaticMesh(NewMesh);

	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	Quantity = NewQuantity;
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
