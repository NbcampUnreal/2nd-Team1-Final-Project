// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API UOptionMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnBackButtonClicked();

	UFUNCTION()
	void OnApplyButtonClicked();

	UPROPERTY(meta= (BindWidget))
	class UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* ResolutionComboBox;

	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* WindowModeComboBox;

	UPROPERTY(meta = (BindWidget))
	class UButton* ApplyButton;


};
