// Fill out your copyright notice in the Description page of Project Settings.


#include "RSInteractableLevelTravel.h"
#include "RSDunPlayerCharacter.h"
#include "RSGameInstance.h"
#include "RSSaveGameSubsystem.h"
#include "RSLevelSubsystem.h"

ARSInteractableLevelTravel::ARSInteractableLevelTravel()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	MeshComp->SetupAttachment(SceneComp);
	MeshComp->SetCollisionProfileName("Interactable");

	InteractName = FText::GetEmpty();
	bIsAutoInteract = false;
}

void ARSInteractableLevelTravel::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARSInteractableLevelTravel::Interact(ARSDunPlayerCharacter* Interactor)
{
	// 레벨을 이동하기 전에 세이브 요청을 의미하는 이벤트 디스패처에 바인딩 된 함수를 호출한다.

	if (!Interactor)
	{
		return;
	}

	UGameInstance* CurGameInstance = Interactor->GetGameInstance();
	if (!CurGameInstance)
	{
		return;
	}

	URSSaveGameSubsystem* SaveGameSubsystem = CurGameInstance->GetSubsystem<URSSaveGameSubsystem>();
	if (!SaveGameSubsystem)
	{
		return;
	}

	SaveGameSubsystem->OnSaveRequested.Broadcast();

	URSGameInstance* RSGameInstance = Interactor->GetGameInstance<URSGameInstance>();
	if (RSGameInstance)
	{
		// 레벨 이동
		URSLevelSubsystem* LevelSubsystem = RSGameInstance->GetSubsystem<URSLevelSubsystem>();

		if (LevelSubsystem)
		{
			LevelSubsystem->TravelToLevel(TargetLevel);
		}
	}
}

FText ARSInteractableLevelTravel::GetInteractName() const
{
	return InteractName;
}

bool ARSInteractableLevelTravel::GetIsAutoInteract() const
{
	return bIsAutoInteract;
}

UAnimMontage* ARSInteractableLevelTravel::GetInteractAnimMontage() const
{
	return nullptr;
}

int32 ARSInteractableLevelTravel::GetWeight() const
{
	return 0;
}
