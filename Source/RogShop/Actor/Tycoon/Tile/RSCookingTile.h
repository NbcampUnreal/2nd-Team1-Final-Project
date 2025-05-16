// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSBaseTile.h"
#include "RSCookingTile.generated.h"

class ARSBaseFood;
class ARSTycoonCustomerCharacter;

UENUM()
enum class ECookingState
{
	None,
	Cooking,
	Finish,
};

UCLASS()
class ROGSHOP_API ARSCookingTile : public ARSBaseTile
{
	GENERATED_BODY()

public:
	ARSCookingTile();

	virtual void Interact() override;

private:
	void OrderToCook();
	void Cook(const FName& FoodKey);
	void TakeFood();

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> FoodLocation; //음식이 나오는 위치
	
	ECookingState State;

	UPROPERTY()
	TWeakObjectPtr<ARSBaseFood> CookedFood;
};
