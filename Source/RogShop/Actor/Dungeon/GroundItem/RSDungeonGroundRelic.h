// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSDungeonGroundItem.h"
#include "RSDungeonGroundRelic.generated.h"

class URSBaseRelic;

UCLASS()
class ROGSHOP_API ARSDungeonGroundRelic : public ARSDungeonGroundItem
{
	GENERATED_BODY()

// 상호작용
public:
	virtual void Interact(ARSDunPlayerCharacter* Interactor) override;

public:
	void SetRelicClass(const TSubclassOf<URSBaseRelic> NewRelicClass);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TargetClass", meta = (AllowPrivateAccess = true))
	TSubclassOf<URSBaseRelic> RelicClass;	// 상호작용시 플레이어에게 넘겨줄 무기 블루프린트 클래스
};
