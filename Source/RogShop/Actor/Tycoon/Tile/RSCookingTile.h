// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSBaseTile.h"
#include "TycoonDatas.h"
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

	GENERATED_TILE()
	
	virtual void Interact(ACharacter* InteractCharacter) override;
	virtual void ResetAll() override;

	FTransform GetChefTransform() const { return ChefLocation->GetComponentTransform(); }
	ECookingState GetState() const { return State; }

private:
	void OrderToCook();
	void Cook(FFoodOrder Order);
	void FinishCook();
	void TakeFood(ACharacter* InteractCharacter);

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> FoodLocation; //음식이 나오는 위치

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> ChefLocation; //요리사가 서있는 위치

	UPROPERTY()
	TObjectPtr<ARSBaseFood> CookedFood; //완성되어 배치되있는 음식 

	// 사운드
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> CookingSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> CookingFinishSound;

	UPROPERTY()
	TObjectPtr<UAudioComponent> CookingAudioComponent;
	
	ECookingState State;
	FFoodOrder CookingFoodOrder;
};
