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

    MaxColumn = 5;

    ARSDunPlayerController* RSDunPlayerController = GetOwningPlayer<ARSDunPlayerController>();
    if (RSDunPlayerController)
    {
        RSDunPlayerController->OnRelicAdded.AddDynamic(this, &URSRelicInventoryWidget::UpdateSlots);
    }

    if (RelicSlots && InvecntorySlotWidgetClass)
    {
        RelicSlots->ClearChildren();
        InvecntorySlots.Empty();
    }
}

void URSRelicInventoryWidget::AddSlot()
{
    if (!RelicSlots || !InvecntorySlotWidgetClass)
    {
        RS_LOG_DEBUG("SlotImageWidgetClass Null");
        return;
    }

    URSInventorySlotWidget* NewSlotImage = CreateWidget<URSInventorySlotWidget>(GetWorld(), InvecntorySlotWidgetClass);

    if (NewSlotImage)
    {
        int32 ChildrenCount = RelicSlots->GetChildrenCount();

        int32 Row = ChildrenCount / MaxColumn;
        int32 Col = ChildrenCount % MaxColumn;

        RelicSlots->AddChildToUniformGrid(NewSlotImage, Row, Col);
        InvecntorySlots.Add(NewSlotImage);
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
        AddSlot();

        int32 TargetIndex = InvecntorySlots.Num() - 1;
        if (InvecntorySlots.IsValidIndex(TargetIndex) && InvecntorySlots[TargetIndex]->GetItemDataTableKey() == NAME_None)
        {
            InvecntorySlots[TargetIndex]->SetSlotItemInfo(RelicDataTableKey, Data->ItemIcon, FString(""));
        }
    }
}
