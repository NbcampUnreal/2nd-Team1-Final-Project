// Fill out your copyright notice in the Description page of Project Settings.
#include "RSDunLifeEssenceShop.h"
#include "RSDunPlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"

ARSDunLifeEssenceShop::ARSDunLifeEssenceShop()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneComp);
	StaticMeshComp->SetCollisionProfileName("Interactable");

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComp->SetupAttachment(SceneComp);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxComp->SetupAttachment(SceneComp);

	InteractName = FText::FromString(TEXT("상점"));
	bIsAutoInteract = false;
}

void ARSDunLifeEssenceShop::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(PlayMontageTimerHandle, this, &ARSDunLifeEssenceShop::PlayMontage, 10.f, true);
}

void ARSDunLifeEssenceShop::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(PlayMontageTimerHandle);
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

void ARSDunLifeEssenceShop::PlayMontage()
{
	if (!SkeletalAnim.IsEmpty())
	{
		int32 MontageIndex = FMath::RandRange(0, SkeletalAnim.Num() - 1);

		UAnimInstance* AnimInsntace = SkeletalMeshComp->GetAnimInstance();

		if (SkeletalAnim.IsValidIndex(MontageIndex) && AnimInsntace)
		{
			AnimInsntace->Montage_Play(SkeletalAnim[MontageIndex], 1.f);
		}
	}
}


