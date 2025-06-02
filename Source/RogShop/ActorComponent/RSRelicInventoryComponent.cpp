// Fill out your copyright notice in the Description page of Project Settings.


#include "RSRelicInventoryComponent.h"
#include "RogShop/UtilDefine.h"
#include "RSDataSubsystem.h"
#include "ItemInfoData.h"

URSRelicInventoryComponent::URSRelicInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void URSRelicInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void URSRelicInventoryComponent::AddRelic(FName RelicKey)
{
	if (CheckValidRelicKey(RelicKey))
	{
		RelicList.Add(RelicKey);
	}
	else
	{
		RS_LOG_C("Failed to add item", FColor::Red);
	}
}

bool URSRelicInventoryComponent::CheckValidRelicKey(const FName& RelicKey)
{
	URSDataSubsystem* Data = GetWorld()->GetGameInstance()->GetSubsystem<URSDataSubsystem>();

	if (FItemInfoData* Row = Data->Relic->FindRow<FItemInfoData>(RelicKey, TEXT("Contains DungeonItemData")))
	{
		return true;
	}

	return false;
}
