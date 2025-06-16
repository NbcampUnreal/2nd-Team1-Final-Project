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

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	NONE,
	NormalAttack,
	StrongAttack,
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
	// 슬롯에 무기를 추가한다.
	UFUNCTION(BlueprintCallable)
	void EquipWeaponToSlot(ARSBaseWeapon* WeaponActor);

	// 특정 슬롯의 무기를 버린다.
	void DropWeaponToSlot(EWeaponSlot TargetWeaponSlot);

	// 장착 중인 무기를 장착 해제한다.
	void UnEquipWeaponToCharacter();

	// 무기 교체시 변경할 슬롯을 미리 저장한다.
	// 교체 애니메이션 재생 중 특정 노티파이에서 교체된다.
	void SetChangeTargetSlot(EWeaponSlot TargetWeaponSlot);

	// 현재 설정된 슬롯으로 무기 교체가 가능한지 반환
	bool CanChangeTargetSlot();

	// 특정 슬롯의 무기를 장착한다.
	void EquipWeaponToCharacter();

private:
	// 무기
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<ARSBaseWeapon>> WeaponActors; // 무기 액터

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = true))
	EWeaponSlot WeaponSlot;	// 1-based, 현재 장착중인 무기 슬롯을 의미한다.

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = true))
	int32 WeaponSlotSize;	// 최대 슬롯 수

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = true))
	EWeaponSlot ChangeTargetSlot;

// 공격 관련
public:
	void HandleAttackInput(EAttackType TargetAttackType);

	bool ContinueComboAttack();

	void ResetCombo();

private:
	// 애니메이션을 위한 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	EAttackType AttackType;	// 현재 공격 종류

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	bool bIsAttack;	// 공격 중인 상태

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	bool bComboInputBuffered;	// 공격 중일 때 들어온 입력 버퍼

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	int32 ComboIndex;	// 콤보 공격을 위한 인덱스 값

// 충돌 로직 관리
public:
	void PerformBoxSweepAttack();

	void ResetDamagedActors();

private:
	UPROPERTY()
	TSet<AActor*> DamagedActors;	// 데미지 입은 액터 저장

// 세이브/로드
private:
	UFUNCTION()
	void SaveRequested();
	UFUNCTION()
	void LoadRequested();

};
