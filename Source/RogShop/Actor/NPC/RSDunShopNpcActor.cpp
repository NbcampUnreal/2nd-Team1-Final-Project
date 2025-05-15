// Fill out your copyright notice in the Description page of Project Settings.
#include "RSDunShopNpcActor.h"
#include "RSDunPlayerCharacter.h"
#include "Blueprint/UserWidget.h"

// Sets default values
ARSDunShopNpcActor::ARSDunShopNpcActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ARSDunShopNpcActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARSDunShopNpcActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARSDunShopNpcActor::Interact(ARSDunPlayerCharacter* Interactor)
{
	if (!Interactor || !StoreWidgetClass) return;

	APlayerController* PC = Cast<APlayerController>(Interactor->GetController());
	if (!PC) return;

	UUserWidget* StoreWidget = CreateWidget<UUserWidget>(PC, StoreWidgetClass);
	if (!StoreWidget) return;

	StoreWidget->AddToViewport();

	UE_LOG(LogTemp, Warning, TEXT("Widget opened from NPC"));
}


