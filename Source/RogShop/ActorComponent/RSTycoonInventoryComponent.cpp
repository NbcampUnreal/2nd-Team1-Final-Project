// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonInventoryComponent.h"

#include "ItemSlot.h"
#include "RSIngredientInventoryWidget.h"
#include "RSTycoonPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/DataTable/CookFoodData.h"
#include "RogShop/GameInstanceSubsystem/RSDataSubsystem.h"
#include "RogShop/GameModeBase/RSTycoonGameModeBase.h"
#include "Tycoon/RSTycoonSaveGame.h"


URSTycoonInventoryComponent::URSTycoonInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URSTycoonInventoryComponent::Open()
{
	bIsOpen = true;

	ARSTycoonPlayerController* Controller = Cast<ARSTycoonPlayerController>(GetOwner());
	check(Controller)

	URSIngredientInventoryWidget* InventoryWidget = Controller->GetInventoryWidget();
	InventoryWidget->AddToViewport();

	for (int32 i = 0; i < ItemList.Num(); i++)
	{
		InventoryWidget->UpdateSlots(i, ItemList[i]);
	}
}

void URSTycoonInventoryComponent::Close()
{
	bIsOpen = false;

	ARSTycoonPlayerController* Controller = Cast<ARSTycoonPlayerController>(GetOwner());
	check(Controller)
	Controller->GetInventoryWidget()->RemoveFromParent();
}

void URSTycoonInventoryComponent::SaveItemData()
{
	URSTycoonSaveGame* SaveGame =
		Cast<URSTycoonSaveGame>(UGameplayStatics::CreateSaveGameObject(URSTycoonSaveGame::StaticClass()));

	SaveGame->Ingredients = ItemList;

	UGameplayStatics::SaveGameToSlot(SaveGame, SaveSlotName, 0);
}

void URSTycoonInventoryComponent::LoadItemData()
{
	URSTycoonSaveGame* SaveGame = Cast<URSTycoonSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (SaveGame)
	{
		ItemList = SaveGame->Ingredients;
	}
}

void URSTycoonInventoryComponent::SetInventorySlot(int32 Value)
{
	ARSTycoonPlayerController* Controller = Cast<ARSTycoonPlayerController>(GetOwner());
	check(Controller)

	URSIngredientInventoryWidget* InventoryWidget = Controller->GetInventoryWidget();
	InventoryWidget->CreateSlots(Value, 5);

	for (int i = 0; i < ItemList.Num(); i++)
	{
		InventoryWidget->UpdateSlots(i, ItemList[i]);
	}
}