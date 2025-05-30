// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RSSpawnManager.h"
#include "RSMapGenerator.h"
#include "RSDungeonGameModeBase.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EMapType : uint8
{
	Forest     UMETA(DisplayName = "숲"),
	Desert     UMETA(DisplayName = "사막"),
	Cave       UMETA(DisplayName = "동굴")
};

UCLASS()
class ROGSHOP_API ARSDungeonGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ARSDungeonGameModeBase();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UFUNCTION()
	void OnMapReady();
private:
	void SpawnMap(EMapType MapType);
	
protected:
	UPROPERTY()
	URSSpawnManager* SpawnManager;
	UPROPERTY()
	ARSMapGenerator* MapGeneratorInstance;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Type")
	EMapType CurrentMapType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	TSubclassOf<AActor> ShopNPCClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
	TSubclassOf<AActor> PlatformClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon")
	TSubclassOf<ARSMapGenerator> ForestMapGeneratorClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon")
	TSubclassOf<ARSMapGenerator> DesertMapGeneratorClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon")
	TSubclassOf<ARSMapGenerator> CaveMapGeneratorClass;
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<ACharacter> PlayerClass;

private:
	FTimerHandle WaitForMapHandle;

	
};
