#include "RefrigeratorWidget.h"
#include "RefrigeratorSlotWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

void URefrigeratorWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Grid_Inventory && RefrigeratorSlotWidgetClass)
	{
		for (int32 i = 0; i < 6; ++i)
		{
			URefrigeratorSlotWidget* NewSlot = CreateWidget<URefrigeratorSlotWidget>(this, RefrigeratorSlotWidgetClass);
			if (NewSlot)
			{
				// 아이콘 불러오기
				UTexture2D* DummyTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Engine/EngineResources/DefaultTexture.DefaultTexture"));

				// 수량 지정
				int32 Quantity = 10 * (i + 1);

				// 슬롯 설정
				NewSlot->SetSlotData(DummyTexture, Quantity);

				// 그리드에 추가
				Grid_Inventory->AddChildToUniformGrid(NewSlot, i / 3, i % 3);
			}
		}
	}
}
