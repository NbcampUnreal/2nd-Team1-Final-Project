// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSInventoryRelicSlotWidget.generated.h"

class UUniformGridPanel;
class URSInventorySlotImageWidget;

UCLASS()
class ROGSHOP_API URSInventoryRelicSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, Category = "Relic Slots")
    TSubclassOf<URSInventorySlotImageWidget> SlotImageWidgetClass;

protected:
    virtual void NativeConstruct() override;

private:
    UPROPERTY(meta = (BindWidget))
    UUniformGridPanel* UniformGridPanel_RelicSlots;

    UPROPERTY()
    TArray<URSInventorySlotImageWidget*> SlotImages;

    void CreateSlots(int32 NumSlots, int32 NumColumns);
};
