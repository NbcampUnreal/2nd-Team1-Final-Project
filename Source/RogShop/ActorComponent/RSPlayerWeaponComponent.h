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
	// 무기
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<ARSBaseWeapon>> WeaponActors; // 무기 액터

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = true))
	EWeaponSlot WeaponSlot;	// 1-based, 현재 장착중인 무기 슬롯을 의미한다.

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = true))
	int32 WeaponSlotSize;	// 최대 슬롯 수

	// 애니메이션을 위한 상태
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = true))
	bool bIsAttack;	// 공격 중인 상태

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = true))
	bool bComboInputBuffered;	// 공격 중일 때 들어온 입력 버퍼

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = true))
	int32 ComboIndex;	// 콤보 공격을 위한 인덱스 값

// 충돌 로직 관리
public:
	void StartAttackOverlap();
	void EndAttackOverlap();

protected:
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY()
	TSet<AActor*> DamagedActors;	// 데미지 입은 액터 저장

// 세이브/로드
private:
	UFUNCTION()
	void SaveRequested();
	UFUNCTION()
	void LoadRequested();

private:
	const FString WeaponSaveSlotName = TEXT("WeaponSaveSlot");
};
