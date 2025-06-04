// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSTycoonSaleWidget.generated.h"

class URSTycoonOrderSlotWidget;
struct FFoodOrder;
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
	void AddOrderSlot(const FFoodOrder* Order);
	void RemoveOrderSlot(const FFoodOrder* Order);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
private:
	void UpdateTimeUI();

	UFUNCTION()
	void OnClickStopSales();
	
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
