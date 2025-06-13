// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonInventoryComponent.h"

#include "ItemSlot.h"
#include "RSIngredientInventoryWidget.h"
#include "RSSaveGameSubsystem.h"
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

	UpdateInventoryWidget();
}

void URSTycoonInventoryComponent::Close()
{
	bIsOpen = false;

	ARSTycoonPlayerController* Controller = Cast<ARSTycoonPlayerController>(GetOwner());
	check(Controller)
	Controller->GetInventoryWidget()->RemoveFromParent();
}

void URSTycoonInventoryComponent::UpdateInventoryWidget()
{
	ARSTycoonPlayerController* Controller = Cast<ARSTycoonPlayerController>(GetOwner());
	check(Controller)

	URSIngredientInventoryWidget* InventoryWidget = Controller->GetInventoryWidget();
	check(InventoryWidget)

	if (InventoryWidget->GetSlotNum() != ItemList.Num())
	{
		InventoryWidget->CreateSlots(ItemList.Num(), 5);
	}

	for (int i = 0; i < ItemList.Num(); i++)
	{
		InventoryWidget->UpdateSlots(i, ItemList[i]);
	}
}

void URSTycoonInventoryComponent::SaveItemData()
{
	Super::SaveItemData();

	const FString& SlotName = GetWorld()->GetGameInstance()->GetSubsystem<URSSaveGameSubsystem>()->TycoonSaveSlot;
	URSTycoonSaveGame* SaveGame =
		Cast<URSTycoonSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));

	SaveGame->Ingredients = ItemList;

	UGameplayStatics::SaveGameToSlot(SaveGame, SlotName, 0);
}

void URSTycoonInventoryComponent::LoadItemData()
{
	Super::LoadItemData();

	const FString& SlotName = GetWorld()->GetGameInstance()->GetSubsystem<URSSaveGameSubsystem>()->TycoonSaveSlot;
	URSTycoonSaveGame* SaveGame = Cast<URSTycoonSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	if (SaveGame)
	{
		ItemList = SaveGame->Ingredients;
	}
}

void URSTycoonInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetGameInstance()->GetSubsystem<URSSaveGameSubsystem>()->
	            OnSaveRequested.AddDynamic(this, &URSTycoonInventoryComponent::SaveItemData);
}