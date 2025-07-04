// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSBaseTile.h"
#include "TycoonDatas.h"
#include "RSTableTile.generated.h"

class ARSTycoonCustomerCharacter;

UCLASS()
class ROGSHOP_API ARSTableTile : public ARSBaseTile
{
	GENERATED_BODY()

public:
	ARSTableTile();

	GENERATED_TILE()

	virtual void Interact(ACharacter* InteractCharacter) override;
	virtual void ResetAll() override;
	
	void Sit(ARSTycoonCustomerCharacter* Customer);

	//주문 불가능하면 -1
	int32 GetOrderWaitCustomerIndex();
	//Order를 기다리고 있는 손님이 없으면 -1
	int32 GetFoodWaitCustomerIndex(FFoodOrder Order);
	//못 앉으면 -1
	int32 GetCanSitingLocationIndex() const;
	bool CanSit() const;
	int32 GetMaxPlace() const { return SittingLocations.Num(); }
	FTransform GetSitTransform(int32 Index) const { return SittingLocations[Index]->GetComponentTransform(); }
	ARSTycoonCustomerCharacter* GetSittingCustomer(int32 Index) const { return SittingCustomers[Index].Get(); }

protected:
	virtual void BeginPlay() override;
	
private:
	void Order(ARSTycoonCustomerCharacter* Customer);
	void Serving(ACharacter* InteractCharacter);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<TObjectPtr<USceneComponent>> FoodLocations; //음식이 위치할 곳

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<TObjectPtr<USceneComponent>> SittingLocations; //손님이 앉는 위치들

	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> FoodActors; //배치된 음식

	UPROPERTY()
	TArray<TWeakObjectPtr<ARSTycoonCustomerCharacter>> SittingCustomers; //앉아있는 손님들
};
