// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ECharacterStatus : uint8
{
	NONE		UMETA(DisplayName = "None"),
	HP			UMETA(DisplayName = "체력"),
	MaxHP		UMETA(DisplayName = "최대 체력"),
	MoveSpeed	UMETA(DisplayName = "이동 속도"),
	AttackPower UMETA(DisplayName = "공격력"),
	AttackSpeed UMETA(DisplayName = "공격 속도"),
};