// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION()
	void OnStartButtonClicked();

	UFUNCTION()
	void OnOptionButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();

protected:
	virtual void NativeConstruct() override;

private:

	UPROPERTY(meta= (BindWidget))
	class UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* OptionButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;

	
};
