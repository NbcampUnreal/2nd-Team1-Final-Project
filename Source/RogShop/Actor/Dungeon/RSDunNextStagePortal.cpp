// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunNextStagePortal.h"
#include "Blueprint/UserWidget.h"
#include "RSDunPlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "RSSendIngredientWidget.h"
#include "RSDungeonIngredientInventoryComponent.h"

ARSDunNextStagePortal::ARSDunNextStagePortal()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	MeshComp->SetupAttachment(SceneComp);
	MeshComp->SetCollisionProfileName("Interactable");

	InteractName = FText::FromString(TEXT("다음 스테이지로"));
	bIsAutoInteract = false;
}

void ARSDunNextStagePortal::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARSDunNextStagePortal::Interact(ARSDunPlayerCharacter* Interactor)
{
	if (SendIngredientWidgetClass && Interactor)
	{
		APlayerController* PC = Interactor->GetController<APlayerController>();
		if (!PC)
		{
			return;
		}

		SendIngredientWidgetInstance = CreateWidget<URSSendIngredientWidget>(PC, SendIngredientWidgetClass);
		if (!SendIngredientWidgetInstance)
		{
			return;
		}

		URSDungeonIngredientInventoryComponent* IngredientInventoryComp = Interactor->GetRSDungeonIngredientInventoryComponent();
		if (!IngredientInventoryComp)
		{
			return;
		}

		const TArray<FItemSlot>& IngredientItems = IngredientInventoryComp->GetItems();

		SendIngredientWidgetInstance->CreateSendIngredientSlots(2);
		SendIngredientWidgetInstance->CreatePlayerIngredientSlots(IngredientItems);
		SendIngredientWidgetInstance->AddToViewport();

		FInputModeUIOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
		PC->FlushPressedKeys();
	}
}

FText ARSDunNextStagePortal::GetInteractName() const
{
	return InteractName;
}

bool ARSDunNextStagePortal::GetIsAutoInteract() const
{
	return bIsAutoInteract;
}
