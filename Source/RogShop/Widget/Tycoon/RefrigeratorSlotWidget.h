#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RefrigeratorSlotWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class ROGSHOP_API URefrigeratorSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// 슬롯 초기화 함수
	void SetSlotData(UTexture2D* IconTexture, int32 Quantity);

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* Image_Icon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Quantity;
};
