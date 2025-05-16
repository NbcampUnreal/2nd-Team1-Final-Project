#pragma once
#include "Engine/Engine.h"


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