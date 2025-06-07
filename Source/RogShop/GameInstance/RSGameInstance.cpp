// Fill out your copyright notice in the Description page of Project Settings.


#include "RSGameInstance.h"
#include "RogShop/UtilDefine.h"
#include "Kismet/GameplayStatics.h"

void URSGameInstance::TravelToLevel(const TSoftObjectPtr<UWorld>& TargetLevelAsset) const
{
	if (TargetLevelAsset.IsNull())
	{
		if (bEnableDebugLog)
		{
			RS_LOG_C("TargetLevelAsset is Null", FColor::Red)
		}
		
		return;
	}

	// 패키지 경로 예시 -> /Game/Maps/MyLevel
	FString LevelPath = TargetLevelAsset.ToSoftObjectPath().GetLongPackageName();

	// 레벨 이름만 추출하고 이동
	FName LevelName = FName(*FPackageName::GetShortName(LevelPath));
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

bool URSGameInstance::GetDebugLogEnabled() const
{
	return bEnableDebugLog;
}

void URSGameInstance::SetDebugLogEnabled(bool bEnable)
{
	bEnableDebugLog = bEnable;
}
