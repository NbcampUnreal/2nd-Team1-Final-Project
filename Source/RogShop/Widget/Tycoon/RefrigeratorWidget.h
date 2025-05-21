#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/UniformGridPanel.h"
#include "RefrigeratorWidget.generated.h"

class URefrigeratorSlotWidget;

UCLASS()
class ROGSHOP_API URefrigeratorWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

protected:
    // 슬롯을 배치할 그리드 패널
    UPROPERTY(meta = (BindWidget))
    UUniformGridPanel* Grid_Inventory;

    // 슬롯 클래스 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
    TSubclassOf<URefrigeratorSlotWidget> RefrigeratorSlotWidgetClass;
};
