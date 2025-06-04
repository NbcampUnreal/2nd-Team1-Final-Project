// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSInteractable.h"
#include "RSDunBossRoomPortal.generated.h"

UCLASS()
class ROGSHOP_API ARSDunBossRoomPortal : public AActor, public IRSInteractable
{
	GENERATED_BODY()
	
public:	
	ARSDunBossRoomPortal();

protected:
	virtual void BeginPlay() override;

// 컴포넌트
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> SceneComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> MeshComp;

// 상호작용
public:
	virtual void Interact(ARSDunPlayerCharacter* Interactor) override;

// 이동할 위치 정보
public:
	void SetTargetTransform(FTransform NewTargetTransform) { TargetTransform = NewTargetTransform; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	FTransform TargetTransform;
};
