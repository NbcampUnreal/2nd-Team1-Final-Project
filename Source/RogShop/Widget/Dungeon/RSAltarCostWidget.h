// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSAltarCostWidget.generated.h"

class UTextBlock;

UCLASS()
class ROGSHOP_API URSAltarCostWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

public:
	void ChangeColor(FLinearColor TargetColor);

	void UpdateCost(FString NewCost);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> CostText;
};
