// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RSBaseInventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGSHOP_API URSBaseInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URSBaseInventoryComponent();

protected:
	virtual void BeginPlay() override;

// 데이터 관리
public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual void AddItem(FName ItemKey, int32 Amount = 1);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual void RemoveItem(FName ItemKey, int32 Amount = 1);

	int32 GetAmount(const FName& ItemKey);
	const TMap<FName, int32>& GetItems() const { return ItemMap; }
	int32 GetFilledSize() const { return ItemMap.Num(); }	//현재 채워진 인벤토리 크기
	int32 GetSlotMaxSize() const { return SlotMaxSize; }	//인벤토리 크기

protected:
	bool CheckValidItem(const FName& ItemKey); //유효한 키값인지 검사

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, int32> ItemMap; //가지고 있는 아이템들

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SlotMaxSize; //인벤토리의 최대 크기

// 세이브/로드
public:
	void SaveItemData();

private:
	void LoadItemData();
};
