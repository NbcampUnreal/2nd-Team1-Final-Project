// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSInteractable.h"
#include "RSInteractableLevelTravel.generated.h"

UCLASS()
class ROGSHOP_API ARSInteractableLevelTravel : public AActor, public IRSInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARSInteractableLevelTravel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// 상호작용
public:
	virtual void Interact(ARSDunPlayerCharacter* Interactor) override;

// 레벨 이동
private:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Level", meta = (AllowPrivateAccess = true))
	TWeakObjectPtr<UWorld> TargetLevel;

// 컴포넌트
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> SceneComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> MeshComp;	// 임시, 나중에 나이아가라 등으로 대체될 수 있다.
};
