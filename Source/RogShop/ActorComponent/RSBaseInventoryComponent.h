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

// ������ ����
public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(FName ItemKey, int32 Amount = 1);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem(FName ItemKey, int32 Amount = 1);

	int32 GetAmount(const FName& ItemKey);
	const TMap<FName, int32>& GetItems() const { return ItemMap; }
	int32 GetFilledSize() const { return ItemMap.Num(); }	//���� ä���� �κ��丮 ũ��
	int32 GetSlotMaxSize() const { return SlotMaxSize; }	//�κ��丮 ũ��

protected:
	bool CheckValidItem(const FName& ItemKey); //��ȿ�� Ű������ �˻�

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, int32> ItemMap; //������ �ִ� �����۵�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SlotMaxSize; //�κ��丮�� �ִ� ũ��

// ���̺�/�ε�
public:
	void SaveItemData();

private:
	void LoadItemData();
};
