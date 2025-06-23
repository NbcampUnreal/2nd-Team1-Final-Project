// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CookFoodData.h"
#include "RSTycoonFoodListWidget.generated.h"

class URSTycoonFoodListButtonWidget;
class UButton;
class UTextBlock;
class UImage;
class UScrollBox;
/**
 * 
 */
UCLASS()
class ROGSHOP_API URSTycoonFoodListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void SetFoodInformation(const FCookFoodData& Data);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UScrollBox> FoodListScroll;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> FoodImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> FoodNameText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> IngredientText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> PriceText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> DescriptionText;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URSTycoonFoodListButtonWidget> FoodListButtonClass;
	
};
