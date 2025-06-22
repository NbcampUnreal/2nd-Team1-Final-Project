// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSWeaponInventoryWidget.generated.h"

class URSInventorySlotWidget;

UCLASS()
class ROGSHOP_API URSWeaponInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void UpdateWeaponSlot(int8 WeaponSlotIndex, FName WeaponKey);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<URSInventorySlotWidget> WeaponSlot1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<URSInventorySlotWidget> WeaponSlot2;

// 슬롯 클릭시 재료 드랍
private:
	UFUNCTION()
	void WeaponSlotPress();

	UFUNCTION()
	void WeaponSlotRelease();

	UFUNCTION()
	void WeaponDrop();

	UFUNCTION()
	void SetClickWeaponName(FName NewClickWeaponName);

private:
	float HoldThreshold;    // 마우스 좌클릭 키다운 시간

	FTimerHandle HoldTimerHandle;   // 마우스 좌클릭 타이머

	FName ClickWeaponName;  // 클릭한 슬롯의 재료의 데이터 테이블 RowName
};
