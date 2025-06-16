// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RSBaseAreaGameModeBase.generated.h"

class URSSpawnManager;

UCLASS()
class ROGSHOP_API ARSBaseAreaGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

protected:
    virtual void BeginPlay() override;

public:
    const TArray<FName>& GetSpawnedWeaponRowNames() const;
    void AddSpawnedWeaponRowName(const FName& RowName);

private:
    UPROPERTY()
    TArray<FName> SpawnedWeaponRowNames;

#pragma region SpawnManager
public:
	URSSpawnManager* GetSpawnManager() const;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dungeon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<URSSpawnManager> SpawnManagerClass; // 던전 오브젝트 매니저 클래스

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dungeon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URSSpawnManager> SpawnManager;// 인스턴스
#pragma endregion
};
