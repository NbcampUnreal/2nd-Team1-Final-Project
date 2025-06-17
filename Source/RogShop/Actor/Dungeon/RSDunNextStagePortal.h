// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSInteractable.h"
#include "RSDunNextStagePortal.generated.h"

class UNiagaraComponent;
class UBoxComponent;
class URSSendIngredientWidget;

UCLASS()
class ROGSHOP_API ARSDunNextStagePortal : public AActor, public IRSInteractable
{
	GENERATED_BODY()
	
public:	
	ARSDunNextStagePortal();

protected:
	virtual void BeginPlay() override;

// 컴포넌트
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> SceneComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> NiagaraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BoxComp;	// 상호작용의 충돌체크를 위한 콜리전 용도의 컴포넌트

// 상호작용
public:
	virtual void Interact(ARSDunPlayerCharacter* Interactor) override;

	virtual FText GetInteractName() const override;

	virtual bool GetIsAutoInteract() const override;

	virtual UAnimMontage* GetInteractAnimMontage() const override;

protected:
	FText InteractName;

	bool bIsAutoInteract;

// 상호작용시 띄워줄 UI
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> SendIngredientWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URSSendIngredientWidget> SendIngredientWidgetInstance;
};
