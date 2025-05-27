// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSTycoonWaitWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class ROGSHOP_API URSTycoonWaitWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnClickSalesStartButton();
	
	UFUNCTION()
	void OnClickManagementButton();
	
	UFUNCTION()
	void OnClickOutButton();
	
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UButton* SaleStartButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UButton* ManagementButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UButton* OutButton;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UWorld> BaseAreaLevel;
};
