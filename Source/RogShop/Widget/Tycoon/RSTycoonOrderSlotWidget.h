// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSTycoonOrderSlotWidget.generated.h"

struct FFoodOrder;
struct FIngredientData;
class UProgressBar;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class ROGSHOP_API URSTycoonOrderSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetOrder(const FFoodOrder* Order);
	void StartProgress(FTimerHandle CookHandle);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	TWeakPtr<const FFoodOrder> GetOrder() const { return CurrentOrder; }
	
private:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	TObjectPtr<UImage> FoodImage;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	TObjectPtr<UTextBlock> FoodNameText;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	TObjectPtr<UTextBlock> FoodTimeText;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	TObjectPtr<UProgressBar> FoodProgressBar;

	TWeakPtr<const FFoodOrder> CurrentOrder;
	FTimerHandle ProgressHandle;
};
