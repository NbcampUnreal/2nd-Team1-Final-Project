// Fill out your copyright notice in the Description page of Project Settings.


#include "RSRelicInventoryWidget.h"
#include "RSDunPlayerController.h"
#include "RSDataSubsystem.h"
#include "ItemInfoData.h"
#include "Components/UniformGridPanel.h"
#include "RSInventorySlotWidget.h"
#include "RogShop/UtilDefine.h"

void URSRelicInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    ARSDunPlayerController* RSDunPlayerController = GetOwningPlayer<ARSDunPlayerController>();
    if (RSDunPlayerController)
    {
        RSDunPlayerController->OnRelicAdded.AddDynamic(this, &URSRelicInventoryWidget::UpdateSlots);

        // 24개 슬롯, 4열 기준 생성
        CreateSlots(24, 5);
    }
}

void URSRelicInventoryWidget::CreateSlots(int32 NumSlots, int32 NumColumns)
{
    if (!RelicSlots || !InvecntorySlotWidgetClass)
    {
        RS_LOG_DEBUG("SlotImageWidgetClass Null");
        return;
    }

    RelicSlots->ClearChildren();
    InvecntorySlots.Empty();

    for (int32 i = 0; i < NumSlots; ++i)
    {
        URSInventorySlotWidget* NewSlotImage = CreateWidget<URSInventorySlotWidget>(GetWorld(), InvecntorySlotWidgetClass);

        if (NewSlotImage)
        {
            int32 Row = i / NumColumns;
            int32 Col = i % NumColumns;

            RelicSlots->AddChildToUniformGrid(NewSlotImage, Row, Col);
            InvecntorySlots.Add(NewSlotImage);
        }
    }
}

void URSRelicInventoryWidget::UpdateSlots(FName RelicDataTableKey)
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

    UDataTable* RelicDataTable = DataSubsystem->RelicInfo;
    if (!RelicDataTable)
    {
        return;
    }

    FItemInfoData* Data = RelicDataTable->FindRow<FItemInfoData>(RelicDataTableKey, TEXT("Get RelicInfo"));
    if (Data)
    {
        for (size_t i = 0; i < InvecntorySlots.Num(); ++i)
        {
            FName ItemDataTableKey = InvecntorySlots[i]->GetItemDataTableKey();
            if (ItemDataTableKey == NAME_None)
            {
                InvecntorySlots[i]->SetSlotItemInfo(RelicDataTableKey, Data->ItemIcon, FString(""));
                return;
            }
        }

        // TODO : 비어있는 슬롯이 없는 경우에 대한 처리

    }
}
