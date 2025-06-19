// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonEventViewWidget.h"

#include "Components/TextBlock.h"
#include "RogShop/Object/RSTycoonEvent.h"

void URSTycoonEventViewWidget::Set(URSTycoonEvent* Event)
{
	EventNameText->SetText(FText::FromString(Event->EventName));
	DescriptionText->SetText(FText::FromString(Event->Description));
}
