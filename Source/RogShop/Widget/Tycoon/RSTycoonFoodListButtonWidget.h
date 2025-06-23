// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSTycoonFoodListButtonWidget.generated.h"

class UButton;
class UTextBlock;
struct FCookFoodData;

DECLARE_DELEGATE_OneParam(FOnClickFoodListButton, const FCookFoodData&)


UCLASS()
class ROGSHOP_API URSTycoonFoodListButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Set(const FCookFoodData* NewData);

protected:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnClick();

public:
	FOnClickFoodListButton OnClickFoodListButton;
	
private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> Button;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> FoodNameText;

	const FCookFoodData* Data;
};
