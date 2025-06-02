// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RSBaseInventoryComponent.generated.h"

struct FItemSlot;

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

	// 지정한 수량만큼 아이템을 인벤토리에 추가합니다.
	// @return 아이템 추가에 성공한 인덱스를 반환하며 실패할 경우 -1을 반환합니다.
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual int32 AddItem(FName ItemKey, int32 Amount = 1);

	// 지정한 수량만큼 아이템을 인벤토리에서 제거합니다.
	// @return 아이템 제거에 성공한 인덱스를 반환하며 실패할 경우 -1을 반환합니다.
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual int32 RemoveItem(FName ItemKey, int32 Amount = 1);

	int32 GetQuantity(const FName& ItemKey);
	const TArray<FItemSlot>& GetItems() const { return ItemList; }
	int32 GetSlotMaxSize() const { return SlotMaxSize; }	//인벤토리 크기
	int32 GetFilledSize() const; //현재 채워진 인벤토리 크기

protected:
	bool CheckValidItem(const FName& ItemKey); //유효한 키값인지 검사

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FItemSlot> ItemList; //가지고 있는 아이템들

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SlotMaxSize; //인벤토리의 최대 크기

// 세이브/로드
public:
	void SaveItemData();

private:
	void LoadItemData();
};
