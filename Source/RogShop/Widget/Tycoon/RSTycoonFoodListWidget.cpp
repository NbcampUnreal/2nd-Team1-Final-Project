// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonFoodListWidget.h"

#include "ItemInfoData.h"
#include "RSDataSubsystem.h"
#include "RSDungeonStatusSaveGame.h"
#include "RSSaveGameSubsystem.h"
#include "RSTycoonFoodListButtonWidget.h"
#include "RSTycoonGameModeBase.h"
#include "RSTycoonInventoryComponent.h"
#include "RSTycoonPlayerController.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/TextBlock.h"
#include "RogShop/UtilDefine.h"

void URSTycoonFoodListWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UDataTable* FoodDataTable = GetGameInstance()->GetSubsystem<URSDataSubsystem>()->Food;
	TArray<FName> AllFoodKeys = FoodDataTable->GetRowNames();
	TArray<FCookFoodData*> AllFoods;
	FoodDataTable->GetAllRows<FCookFoodData>(TEXT("Get All Row In FoodListWidget"), AllFoods);

	for (int32 i = 0; i < AllFoods.Num(); i++)
	{
		URSTycoonFoodListButtonWidget* ButtonWidget =
			CreateWidget<URSTycoonFoodListButtonWidget>(GetWorld(), FoodListButtonClass.Get());
		ButtonWidget->Set(AllFoods[i], AllFoodKeys[i]);
		ButtonWidget->OnClickFoodListButton.BindUObject(this, &URSTycoonFoodListWidget::SetFoodInformation);

		UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(FoodListScroll->AddChild(ButtonWidget));
		ScrollBoxSlot->SetPadding(FMargin(0, 0, 0, 2));
	}

	SetFoodInformation(*AllFoods[0], AllFoodKeys[0]);

	MakeButton->OnClicked.AddDynamic(this, &URSTycoonFoodListWidget::OnClickMakeButton);
}

void URSTycoonFoodListWidget::SetFoodInformation(const FCookFoodData& Data, FName Key)
{
	NowOpenKey = Key;
	NowOpenData = &Data;

	FoodImage->SetBrushFromTexture(Data.Image);
	FoodNameText->SetText(FText::FromString(Data.Name));

	URSDataSubsystem* DataSubsystem = GetGameInstance()->GetSubsystem<URSDataSubsystem>();

	MakeButton->SetVisibility(ESlateVisibility::Hidden);

	FString IngredientStr;
	for (auto& Element : Data.NeedIngredients)
	{
		FItemInfoData* ItemInfo = DataSubsystem->IngredientInfo->FindRow<FItemInfoData>(
			Element.Key, TEXT("Find Ingredient In TycoonFoodListWidget"));

		IngredientStr.Append(FString::Printf(TEXT("%s x %d\n"), *ItemInfo->ItemName.ToString(), Element.Value));

		//스탯 조정 가능한 음식이라면 버튼 활성화
		FIngredientDetailData* Detail =
			DataSubsystem->IngredientDetail->FindRow<FIngredientDetailData>(Element.Key,TEXT("Find Ingredient Detail"));
		if (Detail && Detail->Category == EIngredientType::Special)
		{
			MakeButton->SetVisibility(ESlateVisibility::Visible);
		}
	}

	//스탯 상승 음식일 경우 추가 Text
	FString SpecialStr;
	if (MakeButton->GetVisibility() == ESlateVisibility::Visible)
	{
		FCookFoodDetailData* Detail = DataSubsystem->FoodDetail->FindRow<FCookFoodDetailData>(Key, TEXT("Find Food Detail"));
		if (Detail)
		{
			UEnum* StatusEnum = StaticEnum<ECharacterStatus>();

			for (auto& Status : Detail->PlusStatus)
			{
				FText StatName = StatusEnum->GetDisplayNameTextByIndex((int32)Status.Key);
				SpecialStr.Append(StatName.ToString() + TEXT(" : +") + FString::FromInt(Status.Value) + TEXT("\n"));
			}
		}
		else
		{
			RS_LOG_C("FoodDetailDataTable에 해당 Key 데이터가 없습니다.", FColor::Red)
		}
	}
	IngredientStr.Append(SpecialStr);

	IngredientText->SetText(FText::FromString(IngredientStr));

	PriceText->SetText(FText::FromString(FString::Printf(TEXT("가격 : %d"), Data.Price)));
	DescriptionText->SetText(FText::FromString(Data.Description));
}

void URSTycoonFoodListWidget::OnClickMakeButton()
{
	if (NowOpenKey.IsNone() || NowOpenData == nullptr)
	{
		RS_LOG_C("열려있는 레시피가 없습니다", FColor::Red);
		return;
	}

	//만들자마자 섭취

	//사용한 재료 제거
	FCookFoodData* Data = GetGameInstance()->GetSubsystem<URSDataSubsystem>()->Food
										   ->FindRow<FCookFoodData>(NowOpenKey, TEXT("Get FoodData"));
	
	ARSTycoonPlayerController* Controller = GetWorld()->GetFirstPlayerController<ARSTycoonPlayerController>();
	check(Controller)
	
	for (auto& Need : Data->NeedIngredients)
	{
		Controller->GetInventoryComponent()->RemoveItem(Need.Key, Need.Value);
	}
	
	
	const FString SlotName = GetGameInstance()->GetSubsystem<URSSaveGameSubsystem>()->FoodStatusSaveSlotName;
	URSDungeonStatusSaveGame* StatusSave = Cast<URSDungeonStatusSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	if (StatusSave == nullptr)
	{
		StatusSave = Cast<URSDungeonStatusSaveGame>(
			UGameplayStatics::CreateSaveGameObject(URSDungeonStatusSaveGame::StaticClass()));
	}

	const FCookFoodDetailData* Detail = GetGameInstance()->GetSubsystem<URSDataSubsystem>()->FoodDetail->
	                                                       FindRow<FCookFoodDetailData>(
		                                                       NowOpenKey, TEXT("Find FoodDetail In FoodList"));

	//스탯 추가
	for (auto& Status : Detail->PlusStatus)
	{
		switch (Status.Key)
		{
		case ECharacterStatus::MaxHP:
			StatusSave->MaxHP += Status.Value;
			break;

		case ECharacterStatus::MoveSpeed:
			StatusSave->MoveSpeed += Status.Value;
			break;

		case ECharacterStatus::AttackPower:
			StatusSave->AttackPower += Status.Value;
			break;

		case ECharacterStatus::AttackSpeed:
			StatusSave->AttackSpeed += Status.Value;
			break;

		default: ;
		}
		
		//디버그 메세지
		FText StatName = StaticEnum<ECharacterStatus>()->GetDisplayNameTextByIndex((int32)Status.Key);
		RS_LOG_F("스텟이 증가했습니다. %s + %d", *StatName.ToString(), Status.Value);
	}
	
	//사운드
	//알림

	UGameplayStatics::SaveGameToSlot(StatusSave, SlotName, 0);

	

}
