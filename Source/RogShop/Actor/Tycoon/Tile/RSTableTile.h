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
	void Sit(ARSTycoonCustomerCharacter* Customer);
	
	int32 GetOrderWaitCustomerIndex();
	int32 GetFoodWaitCustomerIndex(FFoodOrder Order);
	int32 GetCanSitingLocationIndex() const;
	bool CanSit() const;
	int32 GetMaxPlace() const { return SittingLocations.Num(); }
	FTransform GetSitTransform(int32 Index) const { return SittingLocations[Index]->GetComponentTransform(); }
	ARSTycoonCustomerCharacter* GetSittingCustomer(int32 Index) const { return SittingCustomers[Index]; }

protected:
	virtual void BeginPlay() override;
	
private:
	void Order(ARSTycoonCustomerCharacter* Customer);
	void Serving(ACharacter* InteractCharacter);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<TObjectPtr<USceneComponent>> FoodLocations; //음식이 위치할 곳

	UPROPERTY()
	TArray<TObjectPtr<AActor>> FoodActors; //배치된 음식

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<TObjectPtr<USceneComponent>> SittingLocations; //손님이 앉는 위치들

	UPROPERTY()
	TArray<TObjectPtr<ARSTycoonCustomerCharacter>> SittingCustomers; //앉아있는 손님들
};
