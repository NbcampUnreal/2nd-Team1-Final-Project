// Fill out your copyright notice in the Description page of Project Settings.


#include "RSLevelSubsystem.h"
#include "RSLevelDeveloperSettings.h"
#include "Kismet/GameplayStatics.h"

void URSLevelSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

}

void URSLevelSubsystem::TravelToLevel(ERSLevelCategory TargetLevel) const
{
	if (ERSLevelCategory::NONE == TargetLevel)
	{
		return;
	}

	TSoftObjectPtr<UWorld> TargetLevelAsset = GetLevel(TargetLevel);

	// 패키지 경로 예시 -> /Game/Maps/MyLevel
	FString LevelPath = TargetLevelAsset.ToSoftObjectPath().GetLongPackageName();

	// 레벨 이름만 추출하고 이동
	FName LevelName = FName(*FPackageName::GetShortName(LevelPath));
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

TSoftObjectPtr<UWorld> URSLevelSubsystem::GetLevel(ERSLevelCategory TargetLevel) const
{
	const URSLevelDeveloperSettings* DataSettings = GetDefault<URSLevelDeveloperSettings>();
	if (!DataSettings)
	{
		return nullptr;
	}

	TSoftObjectPtr<UWorld> CurWorld = nullptr;

	switch (TargetLevel)
	{
	case ERSLevelCategory::MainMenu:
		CurWorld = DataSettings->MainMenuLevel;
		break;
	case ERSLevelCategory::BaseArea:
		CurWorld = DataSettings->BaseAreaLevel;
		break;
	case ERSLevelCategory::Dungeon:
		CurWorld = DataSettings->DungeonLevel;
		break;
	case ERSLevelCategory::Tycoon:
		CurWorld = DataSettings->TycoonLevel;
		break;
	default:
		break;
	}

	return CurWorld;
}