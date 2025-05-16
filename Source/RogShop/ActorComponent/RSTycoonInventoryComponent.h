// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RSTycoonInventoryComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROGSHOP_API URSTycoonInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URSTycoonInventoryComponent();

	void Add(const FName& ItemKey, int32 Amount = 1);
	void Remove(const FName& ItemKey, int32 Amount = 1);
	void SaveItemData();

	int32 GetAmount(const FName& ItemKey);
	const TMap<FName, int32>& GetItems() const { return ItemMap; }
	int32 GetFilledSize() const { return ItemMap.Num(); }	//현재 채워진 인벤토리 크기
	int32 GetSize() const { return Size; }					//인벤토리 크기

protected:
	virtual void BeginPlay() override;

private:
	void LoadItemData();
	bool CheckValidItem(const FName& ItemKey); //유효한 키값인지 검사

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, int32> ItemMap; //가지고 있는 아이템들

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Size = 5; //인벤토리의 최대 크기

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> WidgetType;

	TObjectPtr<UUserWidget> Widget;
};
