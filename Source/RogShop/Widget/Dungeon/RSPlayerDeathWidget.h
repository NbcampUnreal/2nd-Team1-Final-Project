// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSPlayerDeathWidget.generated.h"

class UButton;

UCLASS()
class ROGSHOP_API URSPlayerDeathWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnBaseAreaButtonClicked();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UButton> BaseAreaButton;

// 버튼 클릭으로 이동 할 레벨
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UWorld> BaseAreaLevelAsset;
};
