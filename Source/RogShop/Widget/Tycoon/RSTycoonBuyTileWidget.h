// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSTycoonBuyTileWidget.generated.h"

class UButton;
class ARSBaseTile;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class ROGSHOP_API URSTycoonBuyTileWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetInfo(ARSBaseTile* Tile);

protected:
	virtual void NativeOnInitialized() override;
	
private:
	UFUNCTION()
	void OnClickButton();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> Button;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> TileImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> NameText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> PriceText;

private:
	FName TileKey;
};
