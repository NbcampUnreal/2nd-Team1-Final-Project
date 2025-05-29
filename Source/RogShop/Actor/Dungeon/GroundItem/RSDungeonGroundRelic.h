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
	
public:
	// 해당 엑터의 메시 세팅 및 상호작용에 필요한 변수 세팅
	void InitInteractableRelic(FName NewDataTableKey, UStaticMesh* NewMesh, const TSubclassOf<URSBaseRelic> NewRelicClass);

// 상호작용
public:
	virtual void Interact(ARSDunPlayerCharacter* Interactor) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TargetClass", meta = (AllowPrivateAccess = true))
	TSubclassOf<URSBaseRelic> RelicClass;	// 상호작용시 플레이어에게 넘겨줄 무기 블루프린트 클래스
};
