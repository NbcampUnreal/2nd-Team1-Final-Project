// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSInventoryIngredientSlotWidget.generated.h"

class UUniformGridPanel;
class URSInventorySlotImageWidget;

struct FItemSlot;

UCLASS()
class ROGSHOP_API URSInventoryIngredientSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

public:
    void CreateSlots(int32 NumSlots, int32 NumColumns);

    UFUNCTION()
    void UpdateSlots(int32 IngredientSlotIndex, FItemSlot IngredientItemSlot);

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
    TObjectPtr<UUniformGridPanel> IngredientSlots;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<URSInventorySlotImageWidget> InvecntorySlotWidgetClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
    TArray<URSInventorySlotImageWidget*> InvecntorySlots;
};
