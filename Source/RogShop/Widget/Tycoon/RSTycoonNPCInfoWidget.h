// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSTycoonNPCInfoWidget.generated.h"

class ARSTycoonNPC;
class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class ROGSHOP_API URSTycoonNPCInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetNPC(ARSTycoonNPC* NPC);

protected:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnClickSellButton();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* NameText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UButton* SellButton;

	TWeakObjectPtr<ARSTycoonNPC> InfoNPC;
};
