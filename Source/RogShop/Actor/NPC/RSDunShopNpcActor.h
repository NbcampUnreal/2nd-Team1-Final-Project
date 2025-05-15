// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSInteractable.h" // 인터페이스 헤더
#include "RSDunShopNpcActor.generated.h"

UCLASS()
class ROGSHOP_API ARSDunShopNpcActor : public AActor, public IRSInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARSDunShopNpcActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(ARSDunPlayerCharacter* Interactor) override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> StoreWidgetClass;
};
