// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSInteractable.h"
#include "RSInteractableWeapon.generated.h"

class ARSDungeonItemBase;
class UBoxComponent;

UCLASS()
class ROGSHOP_API ARSInteractableWeapon : public AActor, public IRSInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARSInteractableWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// 해당 엑터의 메시 세팅 및 상호작용에 필요한 변수 세팅
	void InitInteractableWeapon(UStaticMesh* NewMesh, const TSubclassOf<ARSDungeonItemBase> NewWeaponClass);

// 상호작용
public:
	virtual void Interact(ARSDunPlayerCharacter* Interactor) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TSubclassOf<ARSDungeonItemBase> WeaponClass;	// 상호작용시 플레이어에게 넘겨줄 무기 블루프린트 클래스

// 컴포넌트
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> SceneComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> MeshComp;
};
