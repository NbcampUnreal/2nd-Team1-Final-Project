// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunNextStagePortal.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "Blueprint/UserWidget.h"
#include "RSDunPlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "RSSendIngredientWidget.h"
#include "RSDungeonIngredientInventoryComponent.h"
#include "RSDungeonGameModeBase.h"

ARSDunNextStagePortal::ARSDunNextStagePortal()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	NiagaraComp->SetupAttachment(SceneComp);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxComp->SetupAttachment(SceneComp);
	BoxComp->SetCollisionProfileName("Interactable");

	InteractName = FText::FromString(TEXT("다음 스테이지로"));
	bIsAutoInteract = false;
}

void ARSDunNextStagePortal::BeginPlay()
{
	Super::BeginPlay();
	
	ARSDungeonGameModeBase* DungeonGameMode = GetWorld()->GetAuthGameMode<ARSDungeonGameModeBase>();
	if (DungeonGameMode)
	{
		// 마지막 스테이지인 경우
		int32 MaxStageCount = DungeonGameMode->GetMaxStageCount();
		int32 LevelIndex = DungeonGameMode->GetLevelIndex();
		if ((MaxStageCount - 1) == LevelIndex)
		{
			InteractName = FText::FromString(TEXT("거점으로"));
		}
	}
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

		ARSDungeonGameModeBase* DungeonGameMode = GetWorld()->GetAuthGameMode<ARSDungeonGameModeBase>();
		if (DungeonGameMode)
		{
			// 보스 잡은 횟수 + 1개의 슬롯을 적용한다.
			int32 LevelIndex = DungeonGameMode->GetLevelIndex();

			SendIngredientWidgetInstance->CreateSendIngredientSlots((LevelIndex + 1) * 2);
		}

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

UAnimMontage* ARSDunNextStagePortal::GetInteractAnimMontage() const
{
	return nullptr;
}

int32 ARSDunNextStagePortal::GetWeight() const
{
	return 0;
}
