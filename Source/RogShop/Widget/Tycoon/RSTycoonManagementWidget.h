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

public:
	UFUNCTION()
	void BuyTileBorderSlide();

	UFUNCTION()
	void BuyNPCBorderSlide();

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

#pragma region UI Animation, NPC Buy

	UFUNCTION(BlueprintCallable)
	void HandleWaiterClick();

	UFUNCTION(BlueprintCallable)
	void HandleChefClick();

	// BP 노드 실행용 함수, BP 빠지면 이 함수도 필요없음
	UFUNCTION(BlueprintImplementableEvent)
	void OnWaiterClicked_BP();

	UFUNCTION(BlueprintImplementableEvent)
	void OnChefClicked_BP();

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

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URSTycoonBuyNPCWidget> BuyNPCWidget_Waiter;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URSTycoonBuyNPCWidget> BuyNPCWidget_Chef;

#pragma endregion

private:
	UFUNCTION()
	void OnClickExpandTile();

	UFUNCTION()
	void OnClickWaitMode();

	bool bIsBuyTileBorderValid = false;
	bool bIsBuyNPCBorderValid = false;
};
