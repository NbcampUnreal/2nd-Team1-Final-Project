#pragma once
#include "Engine/Engine.h"


#define RS_LOG(Str) \
	UE_LOG(LogTemp, Warning, TEXT(Str))\
	if (GEngine) \
	{	\
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT(Str)); \
	}

#define RS_LOG_S(Str) \
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Str) \
	if (GEngine) \
	{	\
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, Str); \
	}

#define RS_LOG_F(FormatString, ...) \
	UE_LOG(LogTemp, Warning, TEXT(FormatString), ##__VA_ARGS__) \
	if (GEngine) \
	{	\
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT(FormatString), ##__VA_ARGS__)); \
	}