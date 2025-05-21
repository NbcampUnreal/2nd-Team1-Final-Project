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

// ��ȣ�ۿ�
public:
	virtual void Interact(ARSDunPlayerCharacter* Interactor) override;

// ���� �̵�
private:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Level", meta = (AllowPrivateAccess = true))
	TWeakObjectPtr<UWorld> TargetLevel;

// ������Ʈ
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> SceneComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> MeshComp;	// �ӽ�, ���߿� ���̾ư��� ������ ��ü�� �� �ִ�.
};
