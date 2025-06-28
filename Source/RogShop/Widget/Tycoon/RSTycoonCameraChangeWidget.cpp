// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonCameraChangeWidget.h"

#include "Animation/UMGSequencePlayer.h"
#include "Animation/WidgetAnimation.h"
#include "RogShop/UtilDefine.h"

void URSTycoonCameraChangeWidget::Active(TFunction<void()> Callback)
{
	AddToViewport(5);
	
	auto StartAnimation = PlayAnimation(ChangeStartAnimation);
	float StartAnimationDuration = StartAnimation->GetAnimation()->GetEndTime();
	FTimerHandle StartTimer;
	
	GetWorld()->GetTimerManager().SetTimer(StartTimer, [&, Callback]()
	{
		Callback();
		
		auto EndAnimation = PlayAnimation(ChangeEndAnimation);
		float EndAnimationDuration = EndAnimation->GetAnimation()->GetEndTime();
		FTimerHandle EndTimer;
		
		GetWorld()->GetTimerManager().SetTimer(EndTimer, [&]()
		{
			RemoveFromParent();
		}, EndAnimationDuration, false);
	}, StartAnimationDuration, false);
}
