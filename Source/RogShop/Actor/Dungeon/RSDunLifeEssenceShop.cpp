// Fill out your copyright notice in the Description page of Project Settings.
#include "RSDunLifeEssenceShop.h"
#include "RSDunPlayerCharacter.h"
#include "Blueprint/UserWidget.h"

ARSDunLifeEssenceShop::ARSDunLifeEssenceShop()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractName = FText::FromString(TEXT("상점"));
	bIsAutoInteract = false;
}

void ARSDunLifeEssenceShop::BeginPlay()
{
	Super::BeginPlay();
}

void ARSDunLifeEssenceShop::Interact(ARSDunPlayerCharacter* Interactor)
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

FText ARSDunLifeEssenceShop::GetInteractName() const
{
	return InteractName;
}

bool ARSDunLifeEssenceShop::GetIsAutoInteract() const
{
	return bIsAutoInteract;
}

UAnimMontage* ARSDunLifeEssenceShop::GetInteractAnimMontage() const
{
	return nullptr;
}


