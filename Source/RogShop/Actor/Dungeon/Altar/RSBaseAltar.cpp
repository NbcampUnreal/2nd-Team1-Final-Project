// Fill out your copyright notice in the Description page of Project Settings.


#include "RSBaseAltar.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

ARSBaseAltar::ARSBaseAltar()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneComp);
	StaticMeshComp->SetCollisionProfileName(TEXT("Interactable"));

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	BoxComp->SetupAttachment(SceneComp);

	CostWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("CostWidget"));
	CostWidget->SetupAttachment(SceneComp);
	CostWidget->SetWidgetSpace(EWidgetSpace::World);

	InteractName = FText::GetEmpty();
	bIsAutoInteract = false;
}

void ARSBaseAltar::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARSBaseAltar::Interact(ARSDunPlayerCharacter* Interactor)
{

}

FText ARSBaseAltar::GetInteractName() const
{
	return InteractName;
}

bool ARSBaseAltar::GetIsAutoInteract() const
{
	return false;
}

UAnimMontage* ARSBaseAltar::GetInteractAnimMontage() const
{
	return nullptr;
}

UUserWidget* ARSBaseAltar::GetCostWidgetObject()
{
	if (CostWidget)
	{
		return CostWidget->GetUserWidgetObject();
	}

	return nullptr;
}

