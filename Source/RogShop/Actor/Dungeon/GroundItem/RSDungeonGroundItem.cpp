// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDungeonGroundItem.h"
#include "Components/BoxComponent.h"

// Sets default values
ARSDungeonGroundItem::ARSDungeonGroundItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	MeshComp->SetupAttachment(SceneComp);
	MeshComp->SetCollisionProfileName(TEXT("Interactable"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetEnableGravity(true);
}

// Called when the game starts or when spawned
void ARSDungeonGroundItem::BeginPlay()
{
	Super::BeginPlay();


	
}

void ARSDungeonGroundItem::Interact(ARSDunPlayerCharacter* Interactor)
{

}

void ARSDungeonGroundItem::RandImpulse()
{
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetEnableGravity(true);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	if (MeshComp && MeshComp->IsSimulatingPhysics())
	{
		// X, Y는 랜덤, Z는 위로 튀게
		FVector RandomImpulse = FVector(
			FMath::FRandRange(-200.f, 200.f),
			FMath::FRandRange(-200.f, 200.f),
			FMath::FRandRange(300.f, 500.f)
		);

		MeshComp->AddImpulse(RandomImpulse, NAME_None, true);
	}
}

