// Fill out your copyright notice in the Description page of Project Settings.


#include "RSRelicInventoryComponent.h"
#include "RogShop/UtilDefine.h"
#include "RSDataSubsystem.h"
#include "ItemInfoData.h"
#include "RSDunPlayerController.h"
#include "GameFramework/Character.h"

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

	ACharacter* CurCharacter = GetOwner<ACharacter>();
	if (!CurCharacter)
	{
		return;
	}

	// UI 갱신되도록 이벤트 디스패처 호출
	ARSDunPlayerController* PC = Cast<ARSDunPlayerController>(CurCharacter->GetController());
	if (!PC)
	{
		return;
	}

	PC->OnRelicAdded.Broadcast(RelicKey);
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
