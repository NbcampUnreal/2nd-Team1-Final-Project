// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunNextStagePortal.h"
#include "Blueprint/UserWidget.h"
#include "RSDunPlayerCharacter.h"
#include "GameFramework/PlayerController.h"

ARSDunNextStagePortal::ARSDunNextStagePortal()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	MeshComp->SetupAttachment(SceneComp);
	MeshComp->SetCollisionProfileName("Interactable");
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

		SendIngredientWidgetInstance = CreateWidget<UUserWidget>(PC, SendIngredientWidgetClass);

		if (!SendIngredientWidgetInstance)
		{
			return;
		}

		SendIngredientWidgetInstance->AddToViewport();

		FInputModeUIOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
		PC->FlushPressedKeys();
	}
}
