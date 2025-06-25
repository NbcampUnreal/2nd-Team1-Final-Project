// Fill out your copyright notice in the Description page of Project Settings.


#include "RSBaseTreasureChest.h"
#include "Components/BoxComponent.h"

ARSBaseTreasureChest::ARSBaseTreasureChest()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComp->SetupAttachment(SceneComp);
	SkeletalMeshComp->SetCollisionProfileName(TEXT("Interactable"));

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxComp->SetupAttachment(SceneComp);
	BoxComp->SetCollisionProfileName(TEXT("BlockAll"));

	InteractName = FText::GetEmpty();
	bIsAutoInteract = false;
}

void ARSBaseTreasureChest::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARSBaseTreasureChest::Interact(ARSDunPlayerCharacter* Interactor)
{
	SkeletalMeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	SkeletalMeshComp->PlayAnimation(ChestOpenAnimMontage, false);

	FTimerHandle InteractDelayTimer;

	float PlayLength = 0.f;
	if (ChestOpenAnimMontage)
	{
		PlayLength = ChestOpenAnimMontage->GetPlayLength();
	}

	GetWorld()->GetTimerManager().SetTimer(InteractDelayTimer, this, &ARSBaseTreasureChest::OpenChest, PlayLength, false);
}

FText ARSBaseTreasureChest::GetInteractName() const
{
	return InteractName;
}

bool ARSBaseTreasureChest::GetIsAutoInteract() const
{
	return bIsAutoInteract;
}

UAnimMontage* ARSBaseTreasureChest::GetInteractAnimMontage() const
{
	return nullptr;
}

void ARSBaseTreasureChest::OpenChest()
{

}
