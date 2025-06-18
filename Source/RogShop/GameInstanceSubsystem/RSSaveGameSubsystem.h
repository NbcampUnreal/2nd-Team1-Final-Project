// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RSSaveGameSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSaveRequested);

class UOptionSaveGame;

UCLASS()
class ROGSHOP_API URSSaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void AddIngredientDungeonToTycoon(FName IngredientKey, int32 Amount);
	
	void DeleteAllSaveFile();

	void DeleteDungeonSaveFile();

	// 던전에 대한 세이브 파일이 모두 존재하는 경우 true 반환
	bool DoesDungeonSaveFileExist();

	// 타이쿤에 대한 세이브 파일이 1개라도 존재하는 경우 true 반환
	bool DoesTycoonSaveFileExist();

public:
	UPROPERTY(BlueprintAssignable)
	FOnSaveRequested OnSaveRequested;	// 저장 요청을 의미하는 이벤트 디스패처

// 세이브 슬롯 네임
public:
	// 던전
	const FString IngredientInventorySaveSlotName = TEXT("IngredientInventorySaveSlot");
	const FString WeaponSaveSlotName = TEXT("WeaponSaveSlot");
	const FString RelicSaveSlotName = TEXT("RelicSaveSlot");
	const FString StatusSaveSlotName = TEXT("StatusSaveSlot");
	const FString DungeonInfoSaveSlotName = TEXT("DungeonInfoSaveSlot");

	// 타이쿤
	const FString TycoonSaveSlot = TEXT("TycoonGame");
	const FString TycoonTileMapSaveSlot = TEXT("TileMapSaveSlot");


	//옵션
	const FString OptionSaveSlotName = TEXT("OptionSaveSlot");
	
};
