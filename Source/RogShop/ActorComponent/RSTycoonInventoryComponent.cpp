// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonInventoryComponent.h"

#include "Blueprint/UserWidget.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/DataTable/CookFoodData.h"
#include "RogShop/GameInstanceSubsystem/RSDataSubsystem.h"
#include "RogShop/GameModeBase/RSTycoonGameModeBase.h"


URSTycoonInventoryComponent::URSTycoonInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URSTycoonInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}
