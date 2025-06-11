// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunBossRoomPortal.h"
#include "RSDunPlayerCharacter.h"

ARSDunBossRoomPortal::ARSDunBossRoomPortal()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	MeshComp->SetupAttachment(SceneComp);
	MeshComp->SetCollisionProfileName("Interactable");

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
}

FText ARSDunBossRoomPortal::GetInteractName() const
{
	return InteractName;
}

bool ARSDunBossRoomPortal::GetIsAutoInteract() const
{
	return bIsAutoInteract;
}

