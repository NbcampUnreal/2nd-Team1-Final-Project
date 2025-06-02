// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSTycoonBuyTileWidget.h"
#include "RSTycoonManagementWidget.generated.h"

class USpinBox;
class UVerticalBox;
class ARSBaseTile;
class UTextBlock;
class UButton;
class UBorder;

//class URSTycoonBuyNPCWidget;

UCLASS()
class ROGSHOP_API URSTycoonManagementWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void PlayBuyTileParentBorderSlide();

	UFUNCTION(BlueprintCallable)
	void PlayBuyNPCParentBorderSlide();

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

	UFUNCTION()
	void HandleWaiterClick();

	UFUNCTION()
	void HandleChefClick();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CreateNPCButton;

	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<URSTycoonBuyNPCWidget> BuyNPCWidget_Waiter;

	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<URSTycoonBuyNPCWidget> BuyNPCWidget_Chef;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> BuyTileParentBorder;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* BuyTileParentBorderSlide;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* BuyTileParentBorderClose;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> BuyNPCBorder;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* BuyNPCParentBorderSlide;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* BuyNPCParentBorderClose;

private:
	UFUNCTION()
	void OnClickExpandTile();

	UFUNCTION()
	void OnClickWaitMode();

	bool bIsBuyTileParentBorderValid = false;
	bool bIsBuyNPCParentBorderValid = false;
};
