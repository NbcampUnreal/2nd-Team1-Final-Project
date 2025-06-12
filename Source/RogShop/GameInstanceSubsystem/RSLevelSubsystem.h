// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RSLevelSubsystem.generated.h"


UENUM(BlueprintType)
enum class ERSLevelCategory : uint8
{
	NONE,
	MainMenu,
	BaseArea,
	Dungeon,
	Tycoon,
};

UCLASS()
class ROGSHOP_API URSLevelSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	UFUNCTION()
	void TravelToLevel(ERSLevelCategory TargetLevel) const;

private:
	// Enum값을 설정하고 값에 따라 스위치 문으로 반환
	TSoftObjectPtr<UWorld> GetLevel(ERSLevelCategory TargetLevel) const;
};
