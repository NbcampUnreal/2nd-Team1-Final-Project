// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RSTycoonGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API ARSTycoonGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	void StartGame();
	void AddOrder(const FString& OrderFoodName);
	// void AddCookedFood(const FString& CookedFoodName);

	const TArray<FString>& GetOrders() const { return OrderedFoodNames; }

protected:
	virtual void BeginPlay() override;

private:
	void CreateCustomer();

private:
	int32 NowCustomerCount; //지금 가게에 있는 손님

	UPROPERTY(VisibleAnywhere)
	TArray<FString> OrderedFoodNames;	//오더 들어온 음식들 이름

	// UPROPERTY(VisibleAnywhere)
	// TArray<FString> CookedFoods;		//완성한 음식들 이름
}; 
