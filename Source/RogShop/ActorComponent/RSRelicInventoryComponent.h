// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RSRelicInventoryComponent.generated.h"

class URSBaseRelic;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGSHOP_API URSRelicInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URSRelicInventoryComponent();

protected:
	virtual void BeginPlay() override;

// 유물 관리
public:	
	void ApplyRelic(FName RelicKey);

	const TArray<FName> GetRelicList() const { return RelicList; }

private:
	void AddRelic(FName RelicKey);

	bool CheckValidRelicKey(const FName& RelicKey);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Relic", meta = (AllowPrivateAccess = true))
	TArray<FName> RelicList;

// 세이브/로드
public:
	UFUNCTION()
	void SaveRelicData();

private:
	void LoadRelicData();

private:
	UPROPERTY()
	TArray<TObjectPtr<URSBaseRelic>> RelicObjectList;

};
