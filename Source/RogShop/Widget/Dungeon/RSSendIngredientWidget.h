// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSSendIngredientWidget.generated.h"

class UButton;

UCLASS()
class ROGSHOP_API URSSendIngredientWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void NextStageTravel();

	UFUNCTION()
	void ExitWidget();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UButton> OKButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UButton> ExitButton;

// 레벨 이동
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UWorld> TargetLevelAsset;
};
