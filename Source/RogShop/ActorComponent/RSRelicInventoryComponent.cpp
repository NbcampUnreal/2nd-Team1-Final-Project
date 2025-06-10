// Fill out your copyright notice in the Description page of Project Settings.


#include "RSRelicInventoryComponent.h"
#include "RogShop/UtilDefine.h"
#include "RSDataSubsystem.h"
#include "ItemInfoData.h"
#include "RSDunPlayerController.h"
#include "RSDunPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "RSSaveGameSubsystem.h"
#include "RSDungeonRelicSaveGame.h"
#include "RSBaseRelic.h"

URSRelicInventoryComponent::URSRelicInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void URSRelicInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	LoadRelicData();
	
	ARSDunPlayerCharacter* OwnerCharacter = GetOwner<ARSDunPlayerCharacter>();
	if (!OwnerCharacter)
	{
		return;
	}
	UGameInstance* CurGameInstance = OwnerCharacter->GetGameInstance();
	if (!CurGameInstance)
	{
		return;
	}

	URSSaveGameSubsystem* SaveGameSubsystem = CurGameInstance->GetSubsystem<URSSaveGameSubsystem>();
	if (!SaveGameSubsystem)
	{
		return;
	}

	SaveGameSubsystem->OnSaveRequested.AddDynamic(this, &URSRelicInventoryComponent::SaveRelicData);
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

	if (FItemInfoData* Row = Data->RelicInfo->FindRow<FItemInfoData>(RelicKey, TEXT("Contains DungeonItemData")))
	{
		return true;
	}

	return false;
}

void URSRelicInventoryComponent::SaveRelicData()
{
	// SaveGame 오브젝트 생성
	URSDungeonRelicSaveGame* RelicSaveGame = Cast<URSDungeonRelicSaveGame>(UGameplayStatics::CreateSaveGameObject(URSDungeonRelicSaveGame::StaticClass()));
	if (!RelicSaveGame)
	{
		return;
	}

	RelicSaveGame->RelicList = this->RelicList;

	// 저장
	UGameplayStatics::SaveGameToSlot(RelicSaveGame, RelicSaveSlotName, 0);
}

void URSRelicInventoryComponent::LoadRelicData()
{
	// 저장된 세이브 로드
	URSDungeonRelicSaveGame* RelicLoadGame = Cast<URSDungeonRelicSaveGame>(UGameplayStatics::LoadGameFromSlot(RelicSaveSlotName, 0));
	if (RelicLoadGame)
	{
		TArray<FName> LoadRelicList = RelicLoadGame->RelicList;
		for (size_t i = 0; i < LoadRelicList.Num(); ++i)
		{
			FName CurRelicName = LoadRelicList[i];

			// 동작 시켜야할 클래스를 찾아온다.
			ARSDunPlayerCharacter* OwnerCharacter = GetOwner<ARSDunPlayerCharacter>();
			if (!OwnerCharacter)
			{
				return;
			}

			FString ObjectString = CurRelicName.ToString() + TEXT("Object");
			FName ObjectName = FName(*ObjectString);

			UGameInstance* CurGameInstance = OwnerCharacter->GetGameInstance();
			if (!CurGameInstance)
			{
				return;
			}

			URSDataSubsystem* DataSubsystem = CurGameInstance->GetSubsystem<URSDataSubsystem>();
			if (!DataSubsystem)
			{
				return;
			}

			UDataTable* RelicClassDataTable = DataSubsystem->RelicDetail;
			if (!RelicClassDataTable)
			{
				return;
			}

			FDungeonRelicData* RelicClassData = RelicClassDataTable->FindRow<FDungeonRelicData>(CurRelicName, TEXT("Get RelicClassData"));
			if (!RelicClassData)
			{
				return;
			}

			if (!RelicClassData->RelicClass)
			{
				return;
			}

			URSBaseRelic* SpawnRelic = NewObject<URSBaseRelic>(OwnerCharacter, ObjectName, EObjectFlags::RF_Transient, RelicClassData->RelicClass);
			if (!SpawnRelic)
			{
				return;
			}

			// 유물 인벤토리에 해당 유물 추가
			AddRelic(CurRelicName);

			// 유물의 로직을 적용
			SpawnRelic->ApplyEffect(OwnerCharacter);
		}
	}
}
