// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonOrderSlotWidget.h"

#include "CookFoodData.h"
#include "RSDataSubsystem.h"
#include "RSTycoonGameModeBase.h"
#include "TycoonDatas.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "RogShop/UtilDefine.h"

void URSTycoonOrderSlotWidget::SetOrder(FFoodOrder Order)
{
	CurrentOrder = Order;
	if (Order == FFoodOrder())
	{
		return;
	}
	
	const FCookFoodData* CurrentData = GetGameInstance()->GetSubsystem<URSDataSubsystem>()->Food->
	                                                        FindRow<FCookFoodData>(Order.FoodKey, TEXT("Get Order In Slot"));

	if (CurrentData == nullptr)
	{
		RS_LOG_C("데이터 접근 실패!! : URSTycoonOrderSlotWidget", FColor::Red);
		return;
	}
	
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

void URSTycoonOrderSlotWidget::FinishProgress()
{
	FoodTimeText->SetText(FText::FromString(TEXT("완료")));
	FoodTimeText->SetColorAndOpacity(FSlateColor(FColor::Orange));
}

void URSTycoonOrderSlotWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!CurrentOrder.Empty() && ProgressHandle.IsValid() && GetWorld()->GetTimerManager().IsTimerActive(ProgressHandle))
	{
		float Time = GetWorld()->GetTimerManager().GetTimerElapsed(ProgressHandle);
		float RemainTime = GetWorld()->GetTimerManager().GetTimerRemaining(ProgressHandle);

		FoodTimeText->SetText(FText::FromString(FString::Printf(TEXT("%02.02f"), RemainTime)));
		FoodProgressBar->SetPercent(Time / (Time + RemainTime));
	}
}
