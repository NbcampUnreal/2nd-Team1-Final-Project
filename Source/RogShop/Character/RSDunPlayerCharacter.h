// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSDunBaseCharacter.h"
#include "RSDunPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class URSPlayerWeaponComponent;
class URSDungeonInventoryComponent;
class UAIPerceptionStimuliSourceComponent;

struct FInputActionValue;

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

// ī�޶� ����
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> CameraComp;

// �ִϸ��̼� ��Ÿ�� ����
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> DodgeMontage; // ������

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> DeathMontage; // ���

// ���� ����
public:
	URSPlayerWeaponComponent* GetRSPlayerWeaponComponent();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = true))
	TObjectPtr<URSPlayerWeaponComponent> WeaponComp;

// �κ��丮 ����
public:
	URSDungeonInventoryComponent* GetRSDungeonInventoryComponent();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = true))
	TObjectPtr<URSDungeonInventoryComponent> InventoryComp;

// ��ȣ�ۿ� ����
private:
	void InteractTrace();

private:
	AActor* InteractActor;	// ����Ʈ���̽��� ���� ã�� ��ȣ�ۿ� ������ ����
	float InteractRadius;	// ����Ʈ���̽��� �� �Ÿ�(������)
	float InteractAngle;	// �÷��̾ ���� ���� ��ȣ�ۿ� �� ���͸� ã�� ����

// AI�ۼ��� �ڱ� �ҽ�
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = true))
	TObjectPtr<UAIPerceptionStimuliSourceComponent> AIPerceptionStimuliSourceComp;

// ���� ����
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
};
