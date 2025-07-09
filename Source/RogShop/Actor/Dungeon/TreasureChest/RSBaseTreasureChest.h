// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSInteractable.h"
#include "RSBaseTreasureChest.generated.h"

class UBoxComponent;

UCLASS()
class ROGSHOP_API ARSBaseTreasureChest : public AActor, public IRSInteractable
{
	GENERATED_BODY()
	
public:	
	ARSBaseTreasureChest();

protected:
	virtual void BeginPlay() override;

	// 상호작용
public:
	virtual void Interact(ARSDunPlayerCharacter* Interactor) override;

	virtual FText GetInteractName() const override;

	virtual bool GetIsAutoInteract() const override;

	virtual UAnimMontage* GetInteractAnimMontage() const override;

	virtual int32 GetWeight() const override;

protected:
	FText InteractName;

	bool bIsAutoInteract;

// 상호작용에 필요한 변수 및 함수
protected:
	UFUNCTION()
	virtual void OpenChest();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim")
	TObjectPtr<UAnimMontage> ChestOpenAnimMontage;

// 컴포넌트
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> SceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComp;	// 상호작용을 위한 콜리전 용도의 컴포넌트

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BoxComp;	// 캐릭터와의 충돌을 위한 콜리전 용도의 컴포넌트
};
