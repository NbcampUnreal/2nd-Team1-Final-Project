#include "RefrigeratorSlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void URefrigeratorSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 예시 초기화
	if (Text_Quantity)
	{
		Text_Quantity->SetText(FText::FromString(TEXT("99")));
	}
}

void URefrigeratorSlotWidget::SetSlotData(UTexture2D* IconTexture, int32 Quantity)
{
	if (Image_Icon && IconTexture)
	{
		Image_Icon->SetBrushFromTexture(IconTexture);
	}

	if (Text_Quantity)
	{
		FString CountStr = FString::Printf(TEXT("%d"), Quantity);
		Text_Quantity->SetText(FText::FromString(CountStr));
	}
}
