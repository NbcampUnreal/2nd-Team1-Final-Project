// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ItemSlot.h"
#include "RSDunPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class URSBossHPBarWidget;
class UProgressBar;
class URSDunMainHUDWidget;
class URSPlayerInventoryWidget;
class ARSDunBaseCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBossHPChange, FName, BossName, float, HPPercent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponSlotChange, int8, WeaponSlotIndex, FName, WeaponKey);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnIngredientChange, int32, IngredientSlotIndex, FItemSlot, IngredientItemSlot);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractableFound, FText, InteractName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRelicAdded, FName, RelicKey);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLifeEssenceChange, int32, NewLifeEssence);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFloatDamage, float, DamageAmount);

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
	UFUNCTION(BlueprintCallable)
	void ShowLoadingUI();

	void InitializeRSDunMainWidget();

	void TogglePlayerInventoryWidget();
	void ToggleInGameMenuWidget();

	UFUNCTION()
	void ShowPlayerDeathWidget(ARSDunBaseCharacter* DiedCharacter);

	void ShowInteractWidget();
	void HideInteractWidget();

	URSDunMainHUDWidget* GetDunMainHudWidget();


private:
	// 로딩 위젯
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> LoadingUIWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUserWidget> LoadingUIWidget;

	// 플레이어 캐릭터의 메인 허드 위젯
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> RSDunMainHUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URSDunMainHUDWidget> RSDunMainHUDWidget;

	// 플레이어 캐릭터의 사망 위젯
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> RSDeathWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUserWidget> RSDeathWidget;

// 캐릭터의 델리게이트에 바인딩하는 함수를 모은 함수
private:
	void BindCharacterDelegates();

// 이벤트 디스패처
public:
	UPROPERTY(BlueprintAssignable)
	FOnBossHPChange OnBossHPChange;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponSlotChange OnWeaponSlotChange;	// WeaponSlot을 변경하는 시점

	UPROPERTY(BlueprintAssignable)
	FOnIngredientChange OnIngredientChange;

	UPROPERTY(BlueprintAssignable)
	FOnRelicAdded OnRelicAdded;

	UPROPERTY(BlueprintAssignable)
	FOnInteractableFound OnInteractableFound;

	UPROPERTY(BlueprintAssignable)
	FOnLifeEssenceChange OnLifeEssenceChange;

	UPROPERTY(BlueprintAssignable)
	FOnHPChange OnHPChange;

	UPROPERTY(BlueprintAssignable)
	FOnMaxHPChange OnMaxHPChange;

	UPROPERTY(BlueprintAssignable)
	FOnFloatDamage OnFloatDamage;
};
