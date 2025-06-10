// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSInteractable.h"
#include "RSDungeonGroundItem.generated.h"

UCLASS()
class ROGSHOP_API ARSDungeonGroundItem : public AActor, public IRSInteractable
{
	GENERATED_BODY()
	
public:	
	ARSDungeonGroundItem();

protected:
	virtual void BeginPlay() override;

// 상호작용
public:
	virtual void Interact(ARSDunPlayerCharacter* Interactor) override;

// 아이템을 랜덤한 방향으로 날린다.
public:
	void RandImpulse();

// 컴포넌트
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> SceneComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> MeshComp;

// 데이터 테이블의 RowName을 ID값으로 사용한다.
protected:
	FName DataTableKey;

};
