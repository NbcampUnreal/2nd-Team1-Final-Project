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

//기본
#define RS_LOG(Str) \
	UE_LOG(LogTemp, Warning, TEXT(Str))\
	if (GEngine) \
	{	\
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT(Str)); \
	}

//Color 있는 버전
#define RS_LOG_C(Str, Color) \
	UE_LOG(LogTemp, Warning, TEXT(Str))\
	if (GEngine) \
	{	\
		GEngine->AddOnScreenDebugMessage(-1, 5.f, Color, TEXT(Str)); \
	}

//String 버전
#define RS_LOG_S(Str) \
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Str) \
	if (GEngine) \
	{	\
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, Str); \
	}

//Format 버전
#define RS_LOG_F(FormatString, ...) \
	UE_LOG(LogTemp, Warning, TEXT(FormatString), ##__VA_ARGS__) \
	if (GEngine) \
	{	\
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT(FormatString), ##__VA_ARGS__)); \
	}

//Format, Color 버전
#define RS_LOG_F_C(FormatString, Color, ...) \
	UE_LOG(LogTemp, Warning, TEXT(FormatString), ##__VA_ARGS__) \
	if (GEngine) \
	{	\
		GEngine->AddOnScreenDebugMessage(-1, 5.f, Color, FString::Printf(TEXT(FormatString), ##__VA_ARGS__)); \
	}

// 위에 로그 찍는 부분들 IsDebugLogEnabled 값 사용해서 찍을 수 있도록 수정 필요
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