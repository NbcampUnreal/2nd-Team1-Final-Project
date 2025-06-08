// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSTycoonFoodBubbleWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class ROGSHOP_API URSTycoonFoodBubbleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetImage(FName FoodKey);
	
private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UImage> FoodImage;
};
