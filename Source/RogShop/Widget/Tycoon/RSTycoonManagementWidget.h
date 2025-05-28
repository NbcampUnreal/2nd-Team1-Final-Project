// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSTycoonManagementWidget.generated.h"

class USpinBox;
class UVerticalBox;
class URSTycoonBuyTileWidget;
class ARSBaseTile;
class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class ROGSHOP_API URSTycoonManagementWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnClickExpandTile();

	UFUNCTION()
	void OnClickWaitMode();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> GoldText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> ExpandTileButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<USpinBox> WidthBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<USpinBox> HeightBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> ReturnBaseAreaButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UVerticalBox> BuyTileParent;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URSTycoonBuyTileWidget> BuyTileWidgetType;

	TArray<URSTycoonBuyTileWidget> BuyTileWidgets;
};
