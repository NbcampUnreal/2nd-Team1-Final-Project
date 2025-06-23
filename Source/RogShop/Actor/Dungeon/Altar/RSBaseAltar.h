// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSInteractable.h"
#include "RSBaseAltar.generated.h"

class UBoxComponent;
class UWidgetComponent;

UCLASS()
class ROGSHOP_API ARSBaseAltar : public AActor, public IRSInteractable
{
	GENERATED_BODY()
	
public:	
	ARSBaseAltar();

protected:
	virtual void BeginPlay() override;

// 상호작용
public:
	virtual void Interact(ARSDunPlayerCharacter* Interactor) override;

	virtual FText GetInteractName() const override;

	virtual bool GetIsAutoInteract() const override;

	virtual UAnimMontage* GetInteractAnimMontage() const override;

protected:
	FText InteractName;

	bool bIsAutoInteract;

// 상호작용에 필요한 변수
protected:
	// 제단 활성화에 필요한 코스트
	int32 Cost;

// 컴포넌트
public:
	UUserWidget* GetCostWidgetObject();
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> SceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;	// 상호작용을 위한 콜리전 용도의 컴포넌트

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BoxComp;	// 캐릭터와의 충돌을 위한 콜리전 용도의 컴포넌트

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> CostWidgetComp;
};
