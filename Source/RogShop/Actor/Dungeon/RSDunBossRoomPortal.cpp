// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunBossRoomPortal.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "RSDunPlayerCharacter.h"
#include "RSDungeonGameModeBase.h"

ARSDunBossRoomPortal::ARSDunBossRoomPortal()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	NiagaraComp->SetupAttachment(SceneComp);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxComp->SetupAttachment(SceneComp);
	BoxComp->SetCollisionProfileName("Interactable");

	InteractName = FText::FromString(TEXT("보스 방으로"));
	bIsAutoInteract = false;
}

void ARSDunBossRoomPortal::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARSDunBossRoomPortal::Interact(ARSDunPlayerCharacter* Interactor)
{
	if (Interactor)
	{
		Interactor->SetActorTransform(TargetTransform);
	}

	ARSDungeonGameModeBase* DungeonGameMode = GetWorld()->GetAuthGameMode<ARSDungeonGameModeBase>();
	if (DungeonGameMode)
	{
		DungeonGameMode->OnBossRoomPortalEntered.Broadcast();
	}
}

FText ARSDunBossRoomPortal::GetInteractName() const
{
	return InteractName;
}

bool ARSDunBossRoomPortal::GetIsAutoInteract() const
{
	return bIsAutoInteract;
}

UAnimMontage* ARSDunBossRoomPortal::GetInteractAnimMontage() const
{
	return nullptr;
}

int32 ARSDunBossRoomPortal::GetWeight() const
{
	return 0;
}

