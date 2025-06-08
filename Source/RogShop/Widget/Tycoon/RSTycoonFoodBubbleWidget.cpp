// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonFoodBubbleWidget.h"

#include "CookFoodData.h"
#include "RSDataSubsystem.h"
#include "Components/Image.h"

void URSTycoonFoodBubbleWidget::SetImage(FName FoodKey)
{
	FCookFoodData* Data = GetGameInstance()->GetSubsystem<URSDataSubsystem>()->Food->
	                                         FindRow<FCookFoodData>(FoodKey, TEXT("Bubble Find Data"));

	FoodImage->SetBrushFromTexture(Data->Image);
}
