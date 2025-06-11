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
	if (IsDebugLogEnabled(GWorld)) \
	{ \
		UE_LOG(LogTemp, Warning, TEXT(Str)); \
		if (GEngine) \
		{ \
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT(Str)); \
		} \
	}

// Color 있는 버전
#define RS_LOG_C(Str, Color) \
	if (IsDebugLogEnabled(GWorld)) \
	{ \
		UE_LOG(LogTemp, Warning, TEXT(Str)); \
		if (GEngine) \
		{ \
			GEngine->AddOnScreenDebugMessage(-1, 5.f, Color, TEXT(Str)); \
		} \
	} 

// FString 버전
#define RS_LOG_S(Str) \
	if (IsDebugLogEnabled(GWorld)) \
	{ \
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Str); \
		if (GEngine) \
		{ \
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, Str); \
		} \
	} 

// Format 버전
#define RS_LOG_F(FormatString, ...) \
	if (IsDebugLogEnabled(GWorld)) \
	{ \
		UE_LOG(LogTemp, Warning, TEXT(FormatString), ##__VA_ARGS__); \
		if (GEngine) \
		{ \
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT(FormatString), ##__VA_ARGS__)); \
		} \
	} 

// Format + Color 버전
#define RS_LOG_F_C(FormatString, Color, ...) \
	if (IsDebugLogEnabled(GWorld)) \
	{ \
		UE_LOG(LogTemp, Warning, TEXT(FormatString), ##__VA_ARGS__); \
		if (GEngine) \
		{ \
			GEngine->AddOnScreenDebugMessage(-1, 5.f, Color, FString::Printf(TEXT(FormatString), ##__VA_ARGS__)); \
		} \
	}

#define RS_LOG_DEBUG(Format, ...) \
	if (IsDebugLogEnabled(GWorld)) \
	{ \
		UE_LOG(LogTemp, Warning, TEXT(Format), ##__VA_ARGS__); \
	}

#define RS_DRAW_DEBUG_SPHERE(World, Center, Radius, Segments, Color, bPersistent, LifeTime, DepthPriority, Thickness) \
	if (IsDebugLogEnabled(World)) \
	{ \
		DrawDebugSphere(World, Center, Radius, Segments, Color, bPersistent, LifeTime, DepthPriority, Thickness); \
	}

#define RS_DRAW_DEBUG_BOX(World, Center, Extent, Rotation, Color, bPersistent, LifeTime, DepthPriority, Thickness) \
	if (IsDebugLogEnabled(World)) \
	{ \
		DrawDebugBox(World, Center, Extent, Rotation, Color, bPersistent, LifeTime, DepthPriority, Thickness); \
	}

#define RS_DRAW_DEBUG_LINE(World, LineStart, LineEnd, Color, bPersistent, LifeTime, DepthPriority, Thickness) \
	if (IsDebugLogEnabled(World)) \
	{ \
		DrawDebugLine(World, LineStart, LineEnd, Color, bPersistent, LifeTime, DepthPriority, Thickness); \
	}
