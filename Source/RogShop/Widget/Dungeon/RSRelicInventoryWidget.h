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
	
protected:
    virtual void NativeConstruct() override;

public:
    void CreateSlots(int32 NumSlots, int32 NumColumns);

    UFUNCTION()
    void UpdateSlots(FName RelicDataTableKey);

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", meta = (BindWidget, AllowPrivateAccess = "true"))
    TObjectPtr<UUniformGridPanel> RelicSlots;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<URSInventorySlotWidget> InvecntorySlotWidgetClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
    TArray<TObjectPtr<URSInventorySlotWidget>> InvecntorySlots;
};
