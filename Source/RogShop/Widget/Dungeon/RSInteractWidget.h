// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSInteractWidget.generated.h"

/**
 * 
 */
class UTextBlock;
class UImage;

UCLASS()
class ROGSHOP_API URSInteractWidget : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> ObjectName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> InteractKey;
};
