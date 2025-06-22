 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSTycoonBuyTileWidget.h"
#include "RSTycoonBuyNPCWidget.h"
#include "RSTycoonManagementWidget.generated.h"

class USpinBox;
class UVerticalBox;
class ARSBaseTile;
class UTextBlock;
class UButton;
class UBorder;

UCLASS()
class ROGSHOP_API URSTycoonManagementWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	
private:
	UFUNCTION()
	void OnClickExpandTile();

	UFUNCTION()
	void OnClickWaitMode();

	UFUNCTION()
	void OnChangeGold(int32 Value);

	UFUNCTION()
	void OnChangeTileSizeBox(float Value);

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
	TObjectPtr<UTextBlock> TilePriceText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> CanTileChangeText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> ReturnBaseAreaButton;

#pragma region UI Animation, NPC Buy
public:
	void OpenBuyTileLayout();
	void CloseBuyTileLayout();
	void OpenBuyNPCLayout();
	void CloseBuyNPCLayout();

	UFUNCTION()
	void OpenAndCloseNPCLayout();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UVerticalBox> BuyTileParent;
	
	
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URSTycoonBuyTileWidget> BuyTileWidgetType;

	UPROPERTY()
	TArray<URSTycoonBuyTileWidget*> BuyTileWidgets;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> BuyTileParentBorder;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* BuyTileBorderSlideAni;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* BuyTileBorderCloseAni;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> BuyNPCBorder;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* BuyNPCBorderSlideAni;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* BuyNPCBorderCloseAni;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CreateNPCButton;

	bool bOpenBuyTileLayout = false;
	bool bOpenBuyNPCLayout = false;
#pragma endregion
};
