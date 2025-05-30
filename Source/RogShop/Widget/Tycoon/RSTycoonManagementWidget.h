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
class UBorder;

UCLASS()
class ROGSHOP_API URSTycoonManagementWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void PlayBuyTileParentBorderSlideIn();

protected:
	virtual void NativeConstruct() override;

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

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> BuyTileParentBorder;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* SlideIn;

private:
	UFUNCTION()
	void OnClickExpandTile();

	UFUNCTION()
	void OnClickWaitMode();

	bool bIsBuyTileParentBorderValid = false;
};
