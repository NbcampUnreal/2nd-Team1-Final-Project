// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSSendIngredientWidget.generated.h"

class UButton;
class UUniformGridPanel;
class URSInventorySlotWidget;
class UCanvasPanel;

struct FItemSlot;

UCLASS()
class ROGSHOP_API URSSendIngredientWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void SendIngredientAndNextStageTravel();

	UFUNCTION()
	void ExitWidget();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UButton> OKButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UButton> ExitButton;

// 재료 슬롯
public:
	void CreateSendIngredientSlots(int32 NumSlots);

	void CreatePlayerIngredientSlots(const TArray<FItemSlot>& IngredientItems);

private:
	UFUNCTION()
	void SendIngredientSlotCheck();

	UFUNCTION()
	void SetClickIngredientName(FName NewClickIngredientName);

	UFUNCTION()
	void SendIngredientSlotPress();

	UFUNCTION()
	void PlayerIngredientSlotPress();

private:
	// 바인드 위젯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UUniformGridPanel> SendIngredientPanel;	// 보낼 재료 슬롯을 배치할 패널

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UUniformGridPanel> PlayerIngredientPanel;	// 플레이어가 획득한 재료 슬롯을 배치할 패널

	// 위젯에 적용할 슬롯
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<URSInventorySlotWidget> InvecntorySlotWidgetClass;	// 슬롯 클래스

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<URSInventorySlotWidget>> SendIngredientSlots;	// 보낼 재료를 보여줄 슬롯

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<URSInventorySlotWidget>> PlayerIngredientSlots;	// 플레이어 재료를 보여줄 슬롯

	FName ClickIngredientName;  // 클릭한 슬롯의 재료의 데이터 테이블 RowName

// 경고창
private:
	void WarningShow();

	UFUNCTION()
	void WarningHide();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UCanvasPanel> WarningCanvasPanel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UButton> WarningOkButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UButton> WarningCancelButton;
};
