// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSInGameMenuWidget.generated.h"

class UButton;

UCLASS()
class ROGSHOP_API URSInGameMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnMainMenuButtonClicked();

	UFUNCTION()
	void OnBaseAreaButtonClicked();

	UFUNCTION()
	void OnCloseButtonClicked ();

// UI
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UButton> MainMenuButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UButton> BaseAreaButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UButton> CloseButton;

// 버튼 클릭으로 이동 할 레벨
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UWorld> MainMenuLevelAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UWorld> BaseAreaLevelAsset;
};
