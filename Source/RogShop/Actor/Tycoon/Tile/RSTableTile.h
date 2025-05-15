// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSBaseTile.h"
#include "RSTableTile.generated.h"

class ARSTycoonCustomerCharacter;

UCLASS()
class ROGSHOP_API ARSTableTile : public ARSBaseTile
{
	GENERATED_BODY()

public:
	ARSTableTile();

	virtual void Interact() override;
	bool Sit(const TArray<ARSTycoonCustomerCharacter*>& Customers);

	FVector GetFoodLocation() const;
	const TArray<TObjectPtr<USceneComponent>>& GetSittingLocations() { return SittingLocations; }

private:
	void Order();
	void Serving();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	TObjectPtr<USceneComponent> FoodLocation; //음식이 위치할 곳

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<TObjectPtr<USceneComponent>> SittingLocations; //손님이 앉는 위치들

	UPROPERTY()
	TArray<TObjectPtr<ARSTycoonCustomerCharacter>> SittingCustomers;
};
