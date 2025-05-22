// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSDungeonItemBase.h"
#include "RSInteractable.h"
#include "RSBaseWeapon.generated.h"

class UBoxComponent;

UCLASS()
class ROGSHOP_API ARSBaseWeapon : public ARSDungeonItemBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARSBaseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

// ������Ʈ
public:
	UBoxComponent* GetBoxComp() const;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> SceneComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<USkeletalMeshComponent> MeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> BoxComp;	// ������ ������ ���� �ݸ��� �뵵�� ������Ʈ

// �ִϸ��̼�
public:
	TSubclassOf<UAnimInstance> GetWeaponAnimInstnace() const;
	UAnimMontage* GetNormalAttack(int32 Index) const;
	const TArray<UAnimMontage*>& GetNormalAttacks() const;
		
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim", meta = (AllowPrivateAccess = true))
	TSubclassOf<UAnimInstance> WeaponAnimInstnace; // ������ �⺻ �̵� �ִ� �ν��Ͻ�

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim", meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UAnimMontage>> NormalAttacks; // ������ �⺻ ���� ��Ÿ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim", meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UAnimMontage>> StrongAttacks; // ������ �� ���� ��Ÿ��

// �浹
public:
	void StartOverlap();
	void EndOverlap();

	float GetWeaponDamage() const;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status", meta = (AllowPrivateAccess = true))
	float WeaponDamage;	// ������ �� ���� ���� ������

// ������ ���̺��� RowName�� ID������ ����Ѵ�.
public:
	FName GetDataTableKey() const;
	void SetDataTableKey(FName NewDataTableKey);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info", meta = (AllowPrivateAccess = true))
	FName DataTableKey;
};
