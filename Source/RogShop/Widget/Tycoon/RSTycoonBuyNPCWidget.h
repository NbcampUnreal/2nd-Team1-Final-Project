// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSTycoonBuyNPCWidget.generated.h"

class ARSTycoonNPC;
class UButton;
class UTextBlock;

UCLASS()
class ROGSHOP_API URSTycoonBuyNPCWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnClickButton();
	
private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> NPCNameText;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> PriceText;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ARSTycoonNPC> SpawnNPCClass;
};
