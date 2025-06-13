// Fill out your copyright notice in the Description page of Project Settings.
#include "RSDunShopNpcActor.h"
#include "RSDunPlayerCharacter.h"
#include "Blueprint/UserWidget.h"

ARSDunShopNpcActor::ARSDunShopNpcActor()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractName = FText::FromString(TEXT("상점"));
	bIsAutoInteract = false;
}

void ARSDunShopNpcActor::BeginPlay()
{
	Super::BeginPlay();
}

void ARSDunShopNpcActor::Interact(ARSDunPlayerCharacter* Interactor)
{
	if (!Interactor || !StoreWidgetClass)
	{
		return;
	}

	APlayerController* PC = Cast<APlayerController>(Interactor->GetController());
	if (!PC)
	{
		return;
	}

	if (!CachedStoreWidget)
	{
		CachedStoreWidget = CreateWidget<UUserWidget>(PC, StoreWidgetClass);
	}

	if (CachedStoreWidget && !CachedStoreWidget->IsInViewport())
	{
		CachedStoreWidget->AddToViewport();
	}
}

FText ARSDunShopNpcActor::GetInteractName() const
{
	return InteractName;
}

bool ARSDunShopNpcActor::GetIsAutoInteract() const
{
	return bIsAutoInteract;
}


