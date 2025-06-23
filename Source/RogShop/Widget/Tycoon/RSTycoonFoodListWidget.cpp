// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonFoodListWidget.h"

#include "ItemInfoData.h"
#include "RSDataSubsystem.h"
#include "RSTycoonFoodListButtonWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/TextBlock.h"

void URSTycoonFoodListWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TArray<FCookFoodData*> AllFoods;
	GetGameInstance()->GetSubsystem<URSDataSubsystem>()->Food->
	                   GetAllRows<FCookFoodData>(TEXT("Get All Row In FoodListWidget"), AllFoods);

	for (auto& FoodData : AllFoods)
	{
		URSTycoonFoodListButtonWidget* ButtonWidget =
			CreateWidget<URSTycoonFoodListButtonWidget>(GetWorld(), FoodListButtonClass.Get());
		ButtonWidget->Set(FoodData);
		ButtonWidget->OnClickFoodListButton.BindUObject(this, &URSTycoonFoodListWidget::SetFoodInformation);

		UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(FoodListScroll->AddChild(ButtonWidget));
		ScrollBoxSlot->SetPadding(FMargin(0, 0, 0, 2));
	}

	SetFoodInformation(*AllFoods[0]);
}

void URSTycoonFoodListWidget::SetFoodInformation(const FCookFoodData& Data)
{
	FoodImage->SetBrushFromTexture(Data.Image);
	FoodNameText->SetText(FText::FromString(Data.Name));

	FString IngredientStr;
	for (auto& Element : Data.NeedIngredients)
	{
		FItemInfoData* ItemInfo = GetGameInstance()->GetSubsystem<URSDataSubsystem>()->IngredientInfo->FindRow<FItemInfoData>(
			Element.Key, TEXT("Find Ingredient In TycoonFoodListWidget"));

		IngredientStr.Append(FString::Printf(TEXT("%s x %d\n"), *ItemInfo->ItemName.ToString(), Element.Value));
	}
	IngredientText->SetText(FText::FromString(IngredientStr));

	PriceText->SetText(FText::FromString(FString::Printf(TEXT("가격 : %d"), Data.Price)));
	DescriptionText->SetText(FText::FromString(Data.Description));
}
