// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSTycoonEventViewWidget.generated.h"

class URSTycoonEvent;
class UTextBlock;
/**
 * 
 */
UCLASS()
class ROGSHOP_API URSTycoonEventViewWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Set(URSTycoonEvent* Event);
	
public:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* EventNameText;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* DescriptionText;
	
};
