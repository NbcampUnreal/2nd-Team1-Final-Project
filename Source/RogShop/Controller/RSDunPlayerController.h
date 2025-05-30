// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RSDunPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class URSDunMainHUDWidget;
class URSPlayerInventoryWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponSlotChange, uint8, SlotIndex, FName, WeaponKey);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnIngredientChange, uint8, SlotIndex, FName, IngredientKey);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHPChange);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMaxHPChange);

UCLASS()
class ROGSHOP_API ARSDunPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ARSDunPlayerController();

	virtual void BeginPlay() override;

// 입력
public:
	void AddMapping();

	void RemoveAllMapping();

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> ToggleInventoryAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> ToggleInGameMenuAction;

// 위젯
public:
	void InitializeRSDunMainWidget();

	void TogglePlayerInventoryWidget();
	void ToggleInGameMenuWidget();

private:
	// 블루프린트에서 지정할 수 있도록 TSubclassOf로 선언
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> RSDunMainHUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URSDunMainHUDWidget> RSDunMainHUDWidget;

// 이벤트 디스패처
public:
	UPROPERTY(BlueprintAssignable)
	FOnWeaponSlotChange OnWeaponSlotChange;	// WeaponSlot을 변경하는 시점

	UPROPERTY(BlueprintAssignable)
	FOnIngredientChange OnIngredientChange;

	UPROPERTY(BlueprintAssignable)
	FOnHPChange OnHPChange;

	UPROPERTY(BlueprintAssignable)
	FOnMaxHPChange OnMaxHPChange;
};
