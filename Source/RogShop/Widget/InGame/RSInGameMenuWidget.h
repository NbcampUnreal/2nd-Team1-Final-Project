// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSInGameMenuWidget.generated.h"

class UButton;
class URSGuideWidget;

UCLASS()
class ROGSHOP_API URSInGameMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void VsibleInGameMenu();

	UFUNCTION()
	void HideInGameMenu();

private:
	UFUNCTION()
	void OnGuideButtonClicked();

	UFUNCTION()
	void OnMainMenuButtonClicked();

	UFUNCTION()
	void OnBaseAreaButtonClicked();

	UFUNCTION()
	void OnCloseButtonClicked();

// 버튼
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UButton> GuideButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UButton> MainMenuButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UButton> BaseAreaButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UButton> CloseButton;

// 가이드 UI
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> GuideWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URSGuideWidget> GuideWidgetInstance;
};
