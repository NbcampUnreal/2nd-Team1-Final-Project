// Fill out your copyright notice in the Description page of Project Settings.


#include "RSIngredientInventoryWidget.h"
#include "RSDunPlayerController.h"
#include "Components/UniformGridPanel.h"
#include "RSInventorySlotWidget.h"
#include "RSDataSubsystem.h"
#include "ItemInfoData.h"
#include "ItemSlot.h"
#include "RSTycoonPlayerController.h"
#include "RogShop/UtilDefine.h"

void URSIngredientInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ARSDunPlayerController* RSDunPlayerController = GetOwningPlayer<ARSDunPlayerController>();
	if (RSDunPlayerController)
	{
		RSDunPlayerController->OnIngredientChange.AddDynamic(this, &URSIngredientInventoryWidget::UpdateSlots);
		
		// 24개 슬롯, 4열 기준 생성
		CreateSlots(24, 4);
	}
}

void URSIngredientInventoryWidget::CreateSlots(int32 NumSlots, int32 NumColumns)
{
	if (!IngredientSlots || !InvecntorySlotWidgetClass)
	{
		RS_LOG_DEBUG("SlotImageWidgetClass Null");
		return;
	}

	IngredientSlots->ClearChildren();
	InvecntorySlots.Empty();

	for (int32 i = 0; i < NumSlots; ++i)
	{
		URSInventorySlotWidget* NewSlotImage = CreateWidget<URSInventorySlotWidget>(GetWorld(), InvecntorySlotWidgetClass);

		if (NewSlotImage)
		{
			int32 Row = i / NumColumns;
			int32 Col = i % NumColumns;

			IngredientSlots->AddChildToUniformGrid(NewSlotImage, Row, Col);
			InvecntorySlots.Add(NewSlotImage);

			// 슬롯 인덱스 설정
			NewSlotImage->SetIsPressable(true);
		}
	}
}

void URSIngredientInventoryWidget::UpdateSlots(int32 IngredientSlotIndex, FItemSlot IngredientItemSlot)
{
	UWorld* CurrentWorld = GetWorld();
	if (!CurrentWorld)
	{
		return;
	}

	UGameInstance* CurrentGameInstance = CurrentWorld->GetGameInstance();
	if (!CurrentGameInstance)
	{
		return;
	}

	URSDataSubsystem* DataSubsystem = CurrentGameInstance->GetSubsystem<URSDataSubsystem>();
	if (!DataSubsystem)
	{
		return;
	}

	UDataTable* IngredientDataTable = DataSubsystem->IngredientInfo;
	if (!IngredientDataTable)
	{
		return;
	}

	FName IngredientKey = IngredientItemSlot.ItemKey;
	int32 ItemCount = IngredientItemSlot.Quantity;

	FItemInfoData* IngredientInfoDataRow = IngredientDataTable->FindRow<FItemInfoData>(IngredientKey, TEXT("Get IngredientInfo"));
	if (IngredientInfoDataRow)
	{
		// TODO : 현재 데이터에 대한 텍스처 정보를 가져온다.
		// nullptr 대신에 텍스처 정보를 넘겨야한다.
		InvecntorySlots[IngredientSlotIndex]->SetSlotItemInfo(IngredientKey, IngredientInfoDataRow->ItemIcon, FString::FromInt(ItemCount));
	}
}
