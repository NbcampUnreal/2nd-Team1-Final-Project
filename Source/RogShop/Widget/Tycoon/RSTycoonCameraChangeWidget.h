// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSTycoonCameraChangeWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API URSTycoonCameraChangeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Active(TFunction<void()> Callback);

private:
	UPROPERTY(EditAnywhere, meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* ChangeStartAnimation;
	
	UPROPERTY(EditAnywhere, meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* ChangeEndAnimation;
};
