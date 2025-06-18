// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonEvent.h"

#include "Components/TextBlock.h"
#include "RogShop/Widget/Tycoon/RSTycoonEventViewWidget.h"


bool URSTycoonEvent::Condition_Implementation(UWorld* World)
{
	return true;
}

void URSTycoonEvent::Fail_Implementation(UWorld* World)
{
}

void URSTycoonEvent::Success_Implementation(UWorld* World)
{
}
