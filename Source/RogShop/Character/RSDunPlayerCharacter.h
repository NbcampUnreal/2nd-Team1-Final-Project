// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSDunBaseCharacter.h"
#include "SkeletalMergingLibrary.h"
#include "RSDunPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class URSPlayerWeaponComponent;
class URSRelicInventoryComponent;
class URSDungeonIngredientInventoryComponent;
class UAIPerceptionStimuliSourceComponent;

struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSaveRequested);

UCLASS()
class ROGSHOP_API ARSDunPlayerCharacter : public ARSDunBaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARSDunPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void OnDeath();

	virtual void IncreaseMaxHP(float Amount) override;
	virtual void DecreaseMaxHP(float Amount) override;

	virtual void IncreaseHP(float Amount) override;
	virtual void DecreaseHP(float Amount) override;

protected:
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void Dodge(const FInputActionValue& value);
	UFUNCTION()
	void Interaction(const FInputActionValue& value);
	UFUNCTION()
	void NormalAttack(const FInputActionValue& value);
	UFUNCTION()
	void StrongAttack(const FInputActionValue& value);
	UFUNCTION()
	void FirstWeaponSlot(const FInputActionValue& value);
	UFUNCTION()
	void SecondWeaponSlot(const FInputActionValue& value);
	UFUNCTION()
	void ToggleInventoryUI(const FInputActionValue& value);
	UFUNCTION()
	void ToggleInGameMenuUI(const FInputActionValue& value);

// 스켈레탈 메시 관련
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SkeletalMesh", meta = (AllowPrivateAccess = true))
	FSkeletalMeshMergeParams SkeletalMeshMergeParams;

// 카메라 관련
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> CameraComp;

// 애니메이션 몽타주 관련
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> DodgeMontage; // 구르기

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> DeathMontage; // 사망

// 무기 관련
public:
	URSPlayerWeaponComponent* GetRSPlayerWeaponComponent();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = true))
	TObjectPtr<URSPlayerWeaponComponent> WeaponComp;

// 유물 관련
public:
	URSRelicInventoryComponent* GetRSRelicInventoryComponent() { return RelicInventoryComp; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Relic", meta = (AllowPrivateAccess = true))
	TObjectPtr<URSRelicInventoryComponent> RelicInventoryComp;

// 인벤토리 관련
public:
	URSDungeonIngredientInventoryComponent* GetRSDungeonIngredientInventoryComponent();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = true))
	TObjectPtr<URSDungeonIngredientInventoryComponent> IngredientInventoryComp;

// 상호작용 관련
private:
	void InteractTrace();

private:
	AActor* InteractActor;	// 라인트레이스를 통해 찾은 상호작용 가능한 액터
	float InteractRadius;	// 라인트레이스를 할 거리(반지름)
	float InteractAngle;	// 플레이어가 가장 먼저 상호작용 할 액터를 찾을 각도

// AI퍼셉션 자극 소스
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAIPerceptionStimuliSourceComponent> AIPerceptionStimuliSourceComp;

// 스탯 관련
public:
	float GetAttackPower() const;
	void IncreaseAttackPower(float Amount);
	void DecreaseAttackPower(float Amount);

	float GetAttackSpeed() const;
	void IncreaseAttackSpeed(float Amount);
	void DecreaseAttackSpeed(float Amount);

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, category = "Status", meta = (AllowPrivateAccess = "true"))
	float AttackPower;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, category = "Status", meta = (AllowPrivateAccess = "true"))
	float AttackSpeed;

// 세이브 및 로드
public:
	UPROPERTY(BlueprintAssignable)
	FOnSaveRequested OnSaveRequested;	// 저장 요청을 의미하는 이벤트 디스패처
};
