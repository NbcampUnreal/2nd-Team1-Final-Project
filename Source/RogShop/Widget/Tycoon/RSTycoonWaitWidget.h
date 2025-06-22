// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSTycoonWaitWidget.generated.h"

class UBorder;
class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class ROGSHOP_API URSTycoonWaitWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetEnableSaleButton(bool Value);
	
protected:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnClickSalesStartButton();
	
	UFUNCTION()
	void OnClickManagementButton();
	
	UFUNCTION()
	void OnClickOutButton();

	UFUNCTION()
	void OnChangeGold(int32 Value);
	
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UButton* SaleStartButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UButton* ManagementButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UButton* OutButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* GoldText;
};
