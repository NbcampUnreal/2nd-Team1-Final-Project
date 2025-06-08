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
#include "Tycoon/NPC/RSTycoonCustomerCharacter.h"
#include "Tycoon/NPC/RSTycoonWaiterCharacter.h"

void URSTycoonSaleWidget::SetGold(int32 Value)
{
	GoldText->SetText(FText::AsNumber(Value));
}

void URSTycoonSaleWidget::SetCustomerCount(int32 Value)
{
	CustomerCountText->SetText(FText::AsNumber(Value));
}

void URSTycoonSaleWidget::AddOrderSlot(FFoodOrder Order)
{
	URSTycoonOrderSlotWidget* Widget = CreateWidget<URSTycoonOrderSlotWidget>(GetOwningPlayer(), OrderSlotWidgetClass.Get());
	Widget->SetOrder(Order);

	OrderSlotParentBox->AddChildToVerticalBox(Widget);
}

void URSTycoonSaleWidget::RemoveOrderSlot(FFoodOrder Order)
{
	if (URSTycoonOrderSlotWidget* OrderSlot = GetOrderSlotWidget(Order))
	{
		OrderSlot->SetOrder(FFoodOrder());
		OrderSlotParentBox->RemoveChild(OrderSlot);
	}
}

void URSTycoonSaleWidget::StartOrderSlotAnimation(FFoodOrder Order, FTimerHandle CookTimer)
{
	if (URSTycoonOrderSlotWidget* OrderSlot = GetOrderSlotWidget(Order))
	{
		OrderSlot->StartProgress(CookTimer);
	}
}

void URSTycoonSaleWidget::StopOrderSlotAnimation(FFoodOrder Order)
{
	if (URSTycoonOrderSlotWidget* OrderSlot = GetOrderSlotWidget(Order))
	{
		OrderSlot->FinishProgress();
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

URSTycoonOrderSlotWidget* URSTycoonSaleWidget::GetOrderSlotWidget(FFoodOrder Order)
{
	for (auto& Widget : OrderSlotParentBox->GetAllChildren())
	{
		if (URSTycoonOrderSlotWidget* OrderSlot = Cast<URSTycoonOrderSlotWidget>(Widget))
		{
			// RS_LOG_F("비교 하는 위젯 Order : %s", *OrderSlot->GetOrder().FoodKey.ToString());
			//
			// RS_LOG_F("비교 하는 위젯 Order Customer : %s", *OrderSlot->GetOrder().Customer->GetName());

			if (OrderSlot->GetOrder() == Order)
			{
				return OrderSlot;
			}
		}
	}

	RS_LOG_C("해당하는 Order의 맞는 슬롯이 없습니다", FColor::Red);
	RS_LOG_F_C("들어온 Order Key : %s", FColor::Red, *Order.FoodKey.ToString());
	RS_LOG_F_C("들어온 Order Customer : %s", FColor::Red, Order.Customer.Get() ? *Order.Customer->GetName() : TEXT("NULL"));

	return nullptr;
}

void URSTycoonSaleWidget::OnClickStopSales()
{
	GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>()->EndSale();
}
