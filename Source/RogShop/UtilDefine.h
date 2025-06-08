#pragma once

#include "Engine/Engine.h"
#include "RSGameInstance.h"
#include "Kismet/GameplayStatics.h"

inline bool IsDebugLogEnabled(UWorld* World)
{
	if (!World)
	{
		return false;
	}

	const URSGameInstance* GI = Cast<URSGameInstance>(UGameplayStatics::GetGameInstance(World));
	return GI && GI->GetDebugLogEnabled();
}

// 기본
#define RS_LOG(Str) \
do { \
	if (IsDebugLogEnabled(GWorld)) \
	{ \
		UE_LOG(LogTemp, Warning, TEXT(Str)); \
		if (GEngine) \
		{ \
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT(Str)); \
		} \
	} \
} while (0)

// Color 있는 버전
#define RS_LOG_C(Str, Color) \
do { \
	if (IsDebugLogEnabled(GWorld)) \
	{ \
		UE_LOG(LogTemp, Warning, TEXT(Str)); \
		if (GEngine) \
		{ \
			GEngine->AddOnScreenDebugMessage(-1, 5.f, Color, TEXT(Str)); \
		} \
	} \
} while (0)

// FString 버전
#define RS_LOG_S(Str) \
do { \
	if (IsDebugLogEnabled(GWorld)) \
	{ \
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Str); \
		if (GEngine) \
		{ \
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, Str); \
		} \
	} \
} while (0)

// Format 버전
#define RS_LOG_F(FormatString, ...) \
do { \
	if (IsDebugLogEnabled(GWorld)) \
	{ \
		UE_LOG(LogTemp, Warning, TEXT(FormatString), ##__VA_ARGS__); \
		if (GEngine) \
		{ \
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT(FormatString), ##__VA_ARGS__)); \
		} \
	} \
} while (0)

// Format + Color 버전
#define RS_LOG_F_C(FormatString, Color, ...) \
do { \
	if (IsDebugLogEnabled(GWorld)) \
	{ \
		UE_LOG(LogTemp, Warning, TEXT(FormatString), ##__VA_ARGS__); \
		if (GEngine) \
		{ \
			GEngine->AddOnScreenDebugMessage(-1, 5.f, Color, FString::Printf(TEXT(FormatString), ##__VA_ARGS__)); \
		} \
	} \
} while (0)

#define RS_LOG_DEBUG(Format, ...) \
do { \
	if (IsDebugLogEnabled(GWorld)) \
	{ \
		UE_LOG(LogTemp, Warning, TEXT(Format), ##__VA_ARGS__); \
	} \
} while(0)

#define RS_DRAW_DEBUG_SPHERE(World, Center, Radius, Segments, Color, bPersistent, LifeTime, DepthPriority, Thickness) \
do { \
	if (IsDebugLogEnabled(World)) \
	{ \
		DrawDebugSphere(World, Center, Radius, Segments, Color, bPersistent, LifeTime, DepthPriority, Thickness); \
	} \
} while (0)