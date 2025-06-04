// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonSaleWidget.h"

#include "RSTycoonGameModeBase.h"
#include "RSTycoonOrderSlotWidget.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "RogShop/UtilDefine.h"
#include "Tycoon/NPC/RSTycoonChefCharacter.h"
#include "Tycoon/NPC/RSTycoonWaiterCharacter.h"

void URSTycoonSaleWidget::SetGold(int32 Value)
{
	GoldText->SetText(FText::AsNumber(Value));
}

void URSTycoonSaleWidget::SetCustomerCount(int32 Value)
{
	CustomerCountText->SetText(FText::AsNumber(Value));
}

void URSTycoonSaleWidget::AddOrderSlot(const FFoodOrder* Order)
{
	URSTycoonOrderSlotWidget* Widget = CreateWidget<URSTycoonOrderSlotWidget>(GetOwningPlayer(), OrderSlotWidgetClass.Get());
	Widget->SetOrder(Order);
	
	OrderSlotParentBox->AddChildToVerticalBox(Widget);
}

void URSTycoonSaleWidget::RemoveOrderSlot(const FFoodOrder* Order)
{
	for (auto& Widget : OrderSlotParentBox->GetAllChildren())
	{
		if (URSTycoonOrderSlotWidget* OrderSlot = Cast<URSTycoonOrderSlotWidget>(Widget))
		{
			if (OrderSlot->GetOrder().Get() && OrderSlot->GetOrder().Get() == Order)
			{
				OrderSlotParentBox->RemoveChild(Widget);
				return;
			}
		}
	}
}

void URSTycoonSaleWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	GetWorld()->GetTimerManager().SetTimer(TimeUIHandle, this, &URSTycoonSaleWidget::UpdateTimeUI, 0.1f, true);
	
	StopSalesButton->OnClicked.AddDynamic(this, &URSTycoonSaleWidget::OnClickStopSales);
}

void URSTycoonSaleWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	GetWorld()->GetTimerManager().ClearTimer(TimeUIHandle);
}

void URSTycoonSaleWidget::UpdateTimeUI()
{
	ARSTycoonGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>();
	check(GameMode)

	//'분'을 기준으로 계산
	int32 Duration = GameMode->GetSaleHourDuration() * 60; //가게가 몇분동안 여는지 계산
	float MulMinute = Duration / GameMode->GetSaleRealPlayMinute(); //실제 시간에 얼마나 곱해져야 하는지
	float RealSecond = GameMode->GetGameTime(); //GetGameTime은 실제로 지난 '초'를 가져옴
	
	float CurrentMinute = GameMode->GetSaleStartHour() * 60 + MulMinute / 60 * RealSecond; 
	int32 Hour = CurrentMinute / 60;
	int32 Minute = static_cast<int>(CurrentMinute) % 60;	

	TimeText->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d"), Hour, Minute)));

	float Percent = (CurrentMinute - GameMode->GetSaleStartHour() * 60) / Duration;
	TimeProgressBar->SetPercent(Percent);
}

void URSTycoonSaleWidget::OnClickStopSales()
{
	GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>()->EndSale();
}
