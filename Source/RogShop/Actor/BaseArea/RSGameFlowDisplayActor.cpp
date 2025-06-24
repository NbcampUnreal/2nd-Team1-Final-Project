// Fill out your copyright notice in the Description page of Project Settings.


#include "RSGameFlowDisplayActor.h"
#include "Blueprint/UserWidget.h"
#include "RSDunPlayerCharacter.h"
#include "GameFramework/PlayerController.h"

ARSGameFlowDisplayActor::ARSGameFlowDisplayActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneComp);
	StaticMeshComp->SetCollisionProfileName(TEXT("Interactable"));

	InteractName = FText::FromString(TEXT("게임 흐름 설명"));
}

void ARSGameFlowDisplayActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARSGameFlowDisplayActor::Interact(ARSDunPlayerCharacter* Interactor)
{
	if (!Interactor)
	{
		return;
	}

	APlayerController* PC = Interactor->GetController<APlayerController>();
	if (!PC)
	{
		return;
	}

	// 위젯이 생성된 상태인 경우
	if (GameFlowDisplayWidgetInstance)
	{
		GameFlowDisplayWidgetInstance->AddToViewport();

		FInputModeUIOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
		PC->FlushPressedKeys();

		return;
	}

	// 생성된 위젯이 없는 경우
	if (GameFlowDisplayWidgetClass)
	{
		GameFlowDisplayWidgetInstance = CreateWidget<UUserWidget>(PC, GameFlowDisplayWidgetClass);
		if (!GameFlowDisplayWidgetInstance)
		{
			return;
		}

		GameFlowDisplayWidgetInstance->AddToViewport();

		FInputModeUIOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
		PC->FlushPressedKeys();
	}
}

FText ARSGameFlowDisplayActor::GetInteractName() const
{
	return InteractName;
}

bool ARSGameFlowDisplayActor::GetIsAutoInteract() const
{
	return false;
}

UAnimMontage* ARSGameFlowDisplayActor::GetInteractAnimMontage() const
{
	return nullptr;
}


