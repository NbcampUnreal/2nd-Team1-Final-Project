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

// �ش� ������ �޽� ���� �� ��ȣ�ۿ뿡 �ʿ��� ���� ����
	void InitInteractableWeapon(UStaticMesh* NewMesh, const TSubclassOf<ARSDungeonItemBase> NewWeaponClass);

// ��ȣ�ۿ�
public:
	virtual void Interact(ARSDunPlayerCharacter* Interactor) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TSubclassOf<ARSDungeonItemBase> WeaponClass;	// ��ȣ�ۿ�� �÷��̾�� �Ѱ��� ���� �������Ʈ Ŭ����

// ������Ʈ
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> SceneComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> MeshComp;
};
