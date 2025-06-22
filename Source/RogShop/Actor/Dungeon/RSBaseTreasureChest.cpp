// Fill out your copyright notice in the Description page of Project Settings.


#include "RSBaseTreasureChest.h"
#include "NiagaraComponent.h"


// Sets default values
ARSBaseTreasureChest::ARSBaseTreasureChest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	MeshComp->SetupAttachment(SceneComp);
	MeshComp->SetCollisionProfileName(TEXT("Interactable"));

	OpenEffectComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("OpenEffect"));
	OpenEffectComp->SetupAttachment(MeshComp);
	OpenEffectComp->SetAutoActivate(false);

	InteractName = FText::GetEmpty();
	bIsAutoInteract = false;
}

// Called when the game starts or when spawned
void ARSBaseTreasureChest::BeginPlay()
{
	Super::BeginPlay();

}


void ARSBaseTreasureChest::Interact(ARSDunPlayerCharacter* Interactor)
{
	if (bIsOpened || !InteractAnimMontage)
		return;

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(InteractAnimMontage);
		bIsOpened = true;

		if (OpenEffectComp)
		{
			OpenEffectComp->Activate(true);
		}


		// TODO: 보상 지급 로직 추가
		switch (TreasureType)
		{
		case ETreasureType::Weapon:
			UE_LOG(LogTemp, Log, TEXT("무기 지급"));
			break;
		case ETreasureType::Relic:
			UE_LOG(LogTemp, Log, TEXT("유물 지급"));
			break;
		case ETreasureType::Gold:
			UE_LOG(LogTemp, Log, TEXT("돈 지급"));
			break;
		}
	}
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
	return InteractAnimMontage;
}

void ARSBaseTreasureChest::InitTreasureInfo(FText NewInteractName, bool NewbIsAutoInteract, FName NewDataTableKey, USkeletalMesh* NewMesh)
{
	InteractName = NewInteractName;

	bIsAutoInteract = NewbIsAutoInteract;

	DataTableKey = NewDataTableKey;

	MeshComp->SetSkeletalMesh(NewMesh);
}

