// Fill out your copyright notice in the Description page of Project Settings.


#include "RSInteractableLevelTravel.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARSInteractableLevelTravel::ARSInteractableLevelTravel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	MeshComp->SetupAttachment(SceneComp);
	MeshComp->SetCollisionProfileName("Interactable");
}

// Called when the game starts or when spawned
void ARSInteractableLevelTravel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARSInteractableLevelTravel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARSInteractableLevelTravel::Interact(ARSDunPlayerCharacter* Interactor)
{
	if (TargetLevel.Get())
	{
		// TODO : 레벨을 이동하기 전에 세이브 처리가 필요하다.
		FString AssetName = TargetLevel->GetName();
		FName LevelName = FName(*AssetName);
		UGameplayStatics::OpenLevel(GetWorld(), LevelName);
	}
}
