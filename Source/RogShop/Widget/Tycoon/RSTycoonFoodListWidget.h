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
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void SetFoodInformation(const FCookFoodData& Data, FName Key);
	void CreateFoodList();
	
	UFUNCTION()
	void OnClickMakeButton();
	
	FString GetSpecialStr(FName FoodKey);
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> MakeButton;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URSTycoonFoodListButtonWidget> FoodListButtonClass;

	UPROPERTY(EditDefaultsOnly, Category="Sound")
	TObjectPtr<USoundBase> MakeSound;
	
private:
	FName NowOpenKey;
	const FCookFoodData* NowOpenData;
	TArray<URSTycoonFoodListButtonWidget*> FoodListButtons;
};
