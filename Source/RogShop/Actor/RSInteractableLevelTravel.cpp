// Fill out your copyright notice in the Description page of Project Settings.


#include "RSInteractableLevelTravel.h"
#include "RSDunPlayerCharacter.h"
#include "RSGameInstance.h"
#include "RSSaveGameSubsystem.h"

ARSInteractableLevelTravel::ARSInteractableLevelTravel()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	MeshComp->SetupAttachment(SceneComp);
	MeshComp->SetCollisionProfileName("Interactable");
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


	// 레벨 이동 함수 호출
	URSGameInstance* RSGameInstance = Interactor->GetGameInstance<URSGameInstance>();
	if (RSGameInstance)
	{
		RSGameInstance->TravelToLevel(TargetLevelAsset);
	}
}
