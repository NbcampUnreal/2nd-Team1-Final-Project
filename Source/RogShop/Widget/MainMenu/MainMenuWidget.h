// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */

class UButton;
class UUserwidget;

UCLASS()
class ROGSHOP_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnStartButtonClicked();

	UFUNCTION()
	void OnLoadButtonClicked();

	UFUNCTION()
	void OnOptionButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();

protected:

	UPROPERTY(meta= (BindWidget))
	class UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* LoadButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* OptionButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> OptionMenuWidgetClass;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;

	
};
