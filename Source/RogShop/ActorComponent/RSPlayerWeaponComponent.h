// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RSPlayerWeaponComponent.generated.h"


class ARSBaseWeapon;

UENUM(BlueprintType)
enum class EWeaponSlot : uint8
{
	NONE,
	FirstWeaponSlot,
	SecondWeaponSlot,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGSHOP_API URSPlayerWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URSPlayerWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:
	void HandleNormalAttackInput();

	bool ContinueComboAttack();

	void ResetCombo();

	UFUNCTION(BlueprintCallable)
	void EquipWeaponToSlot(ARSBaseWeapon* WeaponActor);

	void EquipWeaponToCharacter(EWeaponSlot TargetWeaponSlot);

private:
	// ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<ARSBaseWeapon>> WeaponActors; // ���� ����

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = true))
	EWeaponSlot WeaponSlot;	// 1-based, ���� �������� ���� ������ �ǹ��Ѵ�.

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = true))
	int32 WeaponSlotSize;	// �ִ� ���� ��

	// �ִϸ��̼��� ���� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = true))
	bool bIsAttack;	// ���� ���� ����

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = true))
	bool bComboInputBuffered;	// ���� ���� �� ���� �Է� ����

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = true))
	int32 ComboIndex;	// �޺� ������ ���� �ε��� ��

// �浹 ���� ����
public:
	void StartAttackOverlap();
	void EndAttackOverlap();

protected:
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY()
	TSet<AActor*> DamagedActors;	// ������ ���� ���� ����

// ���̺�/�ε�
private:
	UFUNCTION()
	void SaveRequested();
	UFUNCTION()
	void LoadRequested();

private:
	const FString WeaponSaveSlotName = TEXT("WeaponSaveSlot");
};
