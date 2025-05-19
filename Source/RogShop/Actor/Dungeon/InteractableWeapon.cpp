// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableWeapon.h"
#include "RogShop/UtilDefine.h"
#include "Components/BoxComponent.h"
#include "RSBaseWeapon.h"
#include "RSDunPlayerCharacter.h"
#include "RSPlayerWeaponComponent.h"

// Sets default values
AInteractableWeapon::AInteractableWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	MeshComp->SetupAttachment(SceneComp);
	MeshComp->SetCollisionProfileName("Interactable");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AInteractableWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableWeapon::InitInteractableWeapon(UStaticMesh* NewMesh, const TSubclassOf<ARSDungeonItemBase> NewWeaponClass)
{
	if (!NewMesh)
	{
		RS_LOG_C("Invalid StaticMesh Used", FColor::Red);
	}
	if (!NewWeaponClass)
	{
		RS_LOG_C("Invalid Blueprint Class Used", FColor::Red);
	}

	MeshComp->SetStaticMesh(NewMesh);
	WeaponClass = NewWeaponClass;

	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AInteractableWeapon::Interact(ARSDunPlayerCharacter* Interactor)
{
	if (!Interactor)
	{
		return;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = Interactor;
	SpawnParameters.Instigator = Interactor;

	ARSBaseWeapon* SpawnWeapon = GetWorld()->SpawnActor<ARSBaseWeapon>(WeaponClass, SpawnParameters);

	URSPlayerWeaponComponent* RSPlayerWeaponComponent = Interactor->GetRSPlayerWeaponComponent();
	if (SpawnWeapon && RSPlayerWeaponComponent)
	{
		RSPlayerWeaponComponent->EquipWeaponToSlot(SpawnWeapon);

		Destroy();
	}
}

