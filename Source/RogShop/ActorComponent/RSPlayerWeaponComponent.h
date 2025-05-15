// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RSPlayerWeaponComponent.generated.h"


class ARSBaseWeapon;

UENUM(BlueprintType)
enum class EWeaponSlot : uint8
{
	FirstWeaponSlot,
	SecondWeaponSlot,
	NONE
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGSHOP_API URSPlayerWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URSPlayerWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

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
	EWeaponSlot WeaponSlot;

	// 애니메이션을 위한 상태
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = true))
	bool bIsAttack;	// 공격 중인 상태

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = true))
	bool bComboInputBuffered;	// 공격 중일 때 들어온 입력 버퍼

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = true))
	int32 ComboIndex;	// 콤보 공격을 위한 인덱스 값
};
