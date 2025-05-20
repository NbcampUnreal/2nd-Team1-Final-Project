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
	void FinishCook();
	void TakeFood();

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> FoodLocation; //음식이 나오는 위치

	UPROPERTY()
	TWeakObjectPtr<ARSBaseFood> CookedFood; //완성되어 배치되있는 음식 

	ECookingState State;
	FName CookingFoodKey; //요리하고 있는 음식 키
};
