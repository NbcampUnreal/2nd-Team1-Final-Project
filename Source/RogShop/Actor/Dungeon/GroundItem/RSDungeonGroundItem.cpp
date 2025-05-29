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
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ARSDungeonGroundItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARSDungeonGroundItem::Interact(ARSDunPlayerCharacter* Interactor)
{

}

