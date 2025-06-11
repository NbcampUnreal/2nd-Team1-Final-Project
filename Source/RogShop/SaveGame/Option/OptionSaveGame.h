#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OptionSaveGame.generated.h"

UCLASS()
class ROGSHOP_API UOptionSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite)
    float MasterVolume = 1.0f;

    UPROPERTY(BlueprintReadWrite)
    float BGMVolume = 1.0f;

    UPROPERTY(BlueprintReadWrite)
    float SFXVolume = 1.0f;

    UPROPERTY(BlueprintReadWrite)
    int32 ResolutionIndex = 0;

    UPROPERTY(BlueprintReadWrite)
    int32 WindowModeIndex = 0;
};
