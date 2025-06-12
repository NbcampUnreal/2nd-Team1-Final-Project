// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TycoonDatas.h"
#include "Blueprint/UserWidget.h"
#include "RSTycoonSaleWidget.generated.h"

class URSTycoonOrderSlotWidget;
class UVerticalBox;
class UProgressBar;
class ARSTycoonWaiterCharacter;
class ARSTycoonChefCharacter;
class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class ROGSHOP_API URSTycoonSaleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetGold(int32 Value);
	void SetCustomerCount(int32 Value);
	void AddOrderSlot(FFoodOrder Order);
	void RemoveOrderSlot(FFoodOrder Order);
	void StartOrderSlotAnimation(FFoodOrder Order, FTimerHandle CookTimer);
	void StopOrderSlotAnimation(FFoodOrder Order);
	void RemoveAllOrderSlots();
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
private:
	UFUNCTION()
	void OnClickStopSales();
	
	void UpdateTimeUI();

	URSTycoonOrderSlotWidget* GetOrderSlotWidget(FFoodOrder Order);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* GoldText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* CustomerCountText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* TimeText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UProgressBar* TimeProgressBar; 	//임시, 시계로 바꿀 예정

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UButton* StopSalesButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UVerticalBox* OrderSlotParentBox;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URSTycoonOrderSlotWidget> OrderSlotWidgetClass;
	
	FTimerHandle TimeUIHandle;
};
