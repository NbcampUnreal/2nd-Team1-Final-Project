// Fill out your copyright notice in the Description page of Project Settings.


#include "RSRelicInventoryWidget.h"
#include "Components/UniformGridPanel.h"
#include "RSInventorySlotWidget.h"

void URSRelicInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 24개 슬롯, 4열 기준 생성
    CreateSlots(24, 4);
}

void URSRelicInventoryWidget::CreateSlots(int32 NumSlots, int32 NumColumns)
{
    if (!RelicSlots || !InvecntorySlotWidgetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("SlotImageWidgetClass Null"));
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
