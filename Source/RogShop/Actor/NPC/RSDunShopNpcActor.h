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
	ARSDunShopNpcActor();

protected:
	virtual void BeginPlay() override;

// 상호작용
public:
	virtual void Interact(ARSDunPlayerCharacter* Interactor) override;

	virtual FText GetInteractName() const override;

	virtual bool GetIsAutoInteract() const override;

protected:
	FText InteractName;

	bool bIsAutoInteract;

// 상호작용시 그려낼 UI
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> StoreWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> CachedStoreWidget;
};
