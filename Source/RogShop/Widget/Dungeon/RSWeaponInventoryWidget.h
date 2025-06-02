// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSWeaponInventoryWidget.generated.h"

UENUM()
enum class EWeaponSlotType : uint8
{
	None,
	Slot1,
	Slot2
};

UCLASS()
class ROGSHOP_API URSWeaponInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void UpdateWeaponSlot(int8 WeaponSlotIndex, FName WeaponKey);

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	class UImage* WeaponSlot1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	class UImage* WeaponSlot2;

	FTimerHandle HoldTimerHandle;
	EWeaponSlotType HeldSlotType = EWeaponSlotType::None;

	// 마우스 좌클릭 키다운 시간
	float HoldThreshold = 0.5f; 

	// 슬롯 홀드 완료 시 처리 함수
	void HandleLongPress();
};
