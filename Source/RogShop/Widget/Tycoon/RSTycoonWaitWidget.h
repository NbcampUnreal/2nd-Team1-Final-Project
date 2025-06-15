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

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UBorder* TutorialBorder;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* InDoorTileText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* InTableTileText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* InCookingTileText;
};
