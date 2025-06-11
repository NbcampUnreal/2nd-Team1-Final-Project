// Fill out your copyright notice in the Description page of Project Settings.


#include "RSIngredientInventoryWidget.h"
#include "RSDunPlayerController.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "RSInventorySlotWidget.h"
#include "RSDataSubsystem.h"
#include "ItemInfoData.h"
#include "ItemSlot.h"
#include "RogShop/UtilDefine.h"
#include "RSDunPlayerCharacter.h"
#include "RSDungeonIngredientInventoryComponent.h"

void URSIngredientInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ARSDunPlayerController* RSDunPlayerController = GetOwningPlayer<ARSDunPlayerController>();
	if (RSDunPlayerController)
	{
		RSDunPlayerController->OnIngredientChange.AddDynamic(this, &URSIngredientInventoryWidget::UpdateSlots);
		
		CreateSlots(9, 3);
	}

	HoldThreshold = 0.5f;
	ClickIngredientName = NAME_None;
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
		URSInventorySlotWidget* NewSlot = CreateWidget<URSInventorySlotWidget>(GetWorld(), InvecntorySlotWidgetClass);

		if (NewSlot)
		{
			int32 Row = i / NumColumns;
			int32 Col = i % NumColumns;

			IngredientSlots->AddChildToUniformGrid(NewSlot, Row, Col);
			InvecntorySlots.Add(NewSlot);

			UButton* SlotButton = NewSlot->GetSlotButton();
			if (SlotButton)
			{
				NewSlot->OnSlotClicked.AddDynamic(this, &URSIngredientInventoryWidget::SetClickIngredientName);
				SlotButton->OnPressed.AddDynamic(this, &URSIngredientInventoryWidget::IngredientSlotPress);
				SlotButton->OnReleased.AddDynamic(this, &URSIngredientInventoryWidget::IngredientSlotRelease);
			}
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
		InvecntorySlots[IngredientSlotIndex]->SetSlotItemInfo(IngredientKey, IngredientInfoDataRow->ItemIcon, FString::FromInt(ItemCount));
	}
	else
	{
		// 만약 데이터 테이블에서 현재 키에 해당하는 값이 없을 경우 슬롯을 비운다.
		InvecntorySlots[IngredientSlotIndex]->SetSlotItemInfo(NAME_None, nullptr, "");
	}
}

void URSIngredientInventoryWidget::IngredientSlotPress()
{
	GetWorld()->GetTimerManager().SetTimer(HoldTimerHandle, this, &URSIngredientInventoryWidget::IngredientDrop, HoldThreshold, false);
}

void URSIngredientInventoryWidget::IngredientSlotRelease()
{
	GetWorld()->GetTimerManager().ClearTimer(HoldTimerHandle);

	ClickIngredientName = NAME_None;
}

void URSIngredientInventoryWidget::IngredientDrop()
{
	ARSDunPlayerCharacter* CurCharacter = GetOwningPlayerPawn<ARSDunPlayerCharacter>();
	if (!CurCharacter)
	{
		return;
	}

	URSDungeonIngredientInventoryComponent* IngredientInventoryComp = CurCharacter->GetRSDungeonIngredientInventoryComponent();
	if (!IngredientInventoryComp)
	{
		return;
	}

	IngredientInventoryComp->DropItem(ClickIngredientName);

	RS_LOG("UI 클릭");
}

void URSIngredientInventoryWidget::SetClickIngredientName(FName NewClickIngredientName)
{
	ClickIngredientName = NewClickIngredientName;
}
