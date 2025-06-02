// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSTycoonBuyNPCWidget.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBuyNPCClicked);

UCLASS()
class ROGSHOP_API URSTycoonBuyNPCWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 부모에서 바인딩할 수 있게 public으로 선언
	UPROPERTY(BlueprintAssignable)
	FOnBuyNPCClicked OnClick;

	void SetNPCName(const FText& NewText);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* NPCButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NPCName;

	UFUNCTION()
	void HandleButtonClicked();
};
