// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSRelicInventoryWidget.generated.h"

class UUniformGridPanel;
class URSInventorySlotWidget;

UCLASS()
class ROGSHOP_API URSRelicInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, Category = "Relic Slots")
    TSubclassOf<URSInventorySlotWidget> SlotImageWidgetClass;

protected:
    virtual void NativeConstruct() override;

private:
    UPROPERTY(meta = (BindWidget))
    UUniformGridPanel* UniformGridPanel_RelicSlots;

    UPROPERTY()
    TArray<URSInventorySlotWidget*> SlotImages;

    void CreateSlots(int32 NumSlots, int32 NumColumns);
};
