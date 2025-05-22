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
	// Sets default values for this actor's properties
	ARSDungeonGroundItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

// �ش� ������ �޽� ���� �� ��ȣ�ۿ뿡 �ʿ��� ���� ����
	void InitItemInfo(FName NewDataTableKey, UStaticMesh* NewMesh);

// ��ȣ�ۿ�
public:
	virtual void Interact(ARSDunPlayerCharacter* Interactor) override;

	// ������Ʈ
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> SceneComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> MeshComp;

	// ������ ���̺��� RowName�� ID������ ����Ѵ�.
private:
	FName DataTableKey;

};
