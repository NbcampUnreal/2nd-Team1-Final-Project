// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonFoodListButtonWidget.h"

#include "CookFoodData.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void URSTycoonFoodListButtonWidget::Set(const FCookFoodData* NewData, FName NewKey)
{
	Data = NewData;
	Key = NewKey;

	FoodNameText->SetText(FText::FromString(Data->Name));
}

void URSTycoonFoodListButtonWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button->OnClicked.AddDynamic(this, &URSTycoonFoodListButtonWidget::OnClick);
}

void URSTycoonFoodListButtonWidget::OnClick()
{
	OnClickFoodListButton.ExecuteIfBound(*Data, Key);
}
