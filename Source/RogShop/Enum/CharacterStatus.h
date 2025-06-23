// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ECharacterStatus : uint8
{
	NONE,
	HP,
	MaxHP,
	MoveSpeed,
	AttackPower,
	AttackSpeed,
};