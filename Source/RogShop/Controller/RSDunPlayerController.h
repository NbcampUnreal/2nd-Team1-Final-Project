// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RSDunMainWidget.h"
#include "RSDunPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponSlotChange, uint8, SlotIndex, FName, WeaponKey);

class UInputMappingContext;
class UInputAction;

UCLASS()
class ROGSHOP_API ARSDunPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ARSDunPlayerController();

	virtual void BeginPlay() override;

public:
	void AddMapping();

	void RemoveAllMapping();

	void InitializeRSDunMainWidget();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> DodgeAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> InteractionAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> NormalAttackAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> StrongAttackAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> FirstWeaponSlotAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> SecondWeaponSlotAction;

// ����
public:
	URSDunMainWidget* GetRSDunMainWidget() const { return RSDunMainWidget; }

private:
	// �������Ʈ���� ������ �� �ֵ��� TSubclassOf�� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> RSDunMainWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = true))
	URSDunMainWidget* RSDunMainWidget;

// �̺�Ʈ ����ó
public:
	UPROPERTY(BlueprintAssignable)
	FOnWeaponSlotChange OnWeaponSlotChange;	// WeaponSlot�� �����ϴ� ����
};
