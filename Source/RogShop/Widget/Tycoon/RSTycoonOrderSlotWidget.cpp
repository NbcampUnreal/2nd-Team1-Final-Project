// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonOrderSlotWidget.h"

#include "CookFoodData.h"
#include "RSDataSubsystem.h"
#include "RSTycoonGameModeBase.h"
#include "TycoonDatas.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void URSTycoonOrderSlotWidget::SetOrder(const FFoodOrder* Order)
{
	CurrentOrder = Order;
	const FIngredientData* CurrentData = GetGameInstance()->GetSubsystem<URSDataSubsystem>()->Ingredient->
	                                                        FindRow<FIngredientData>(Order->FoodKey, TEXT("Get Order In Slot"));

	FoodImage->SetBrushFromTexture(CurrentData->Image);
	FoodNameText->SetText(FText::FromString(CurrentData->Name));
	FoodTimeText->SetText(FText::FromString(TEXT("대기 중")));
	FoodProgressBar->SetPercent(0);

	FoodImage->SetIsEnabled(false);
	FoodProgressBar->SetIsEnabled(false);
}

void URSTycoonOrderSlotWidget::StartProgress(FTimerHandle CookHandle)
{
	FoodImage->SetIsEnabled(true);
	FoodProgressBar->SetIsEnabled(true);

	ProgressHandle = CookHandle;
}

void URSTycoonOrderSlotWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (CurrentOrder.Get() && ProgressHandle.IsValid())
	{
		float Time = GetWorld()->GetTimerManager().GetTimerElapsed(ProgressHandle);
		float MaxTime = GetWorld()->GetTimerManager().GetTimerRemaining(ProgressHandle);

		FoodTimeText->SetText(FText::FromString(FString::Printf("%02f : %.02f", MaxTime - Time,  MaxTime - Time)));
		FoodProgressBar->SetPercent(Time / MaxTime);
	}
}
