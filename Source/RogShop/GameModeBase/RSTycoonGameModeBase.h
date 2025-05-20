// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RSTycoonGameModeBase.generated.h"

class ARSTycoonCustomerCharacter;
struct FCookFoodData;
class URSTycoonInventoryComponent;

UCLASS()
class ROGSHOP_API ARSTycoonGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARSTycoonGameModeBase();

	void StartGame();
	void AddOrder(const FName& OrderFoodName);
	void RemoveOrder(const FName& OrderFoodName);
	void RemoveCustomer(ARSTycoonCustomerCharacter* Customer);

	const TArray<FName>& GetOrders() const { return OrderedFoodKeys; }

protected:
	virtual void BeginPlay() override;

private:
	void CreateCustomer();
	bool CanOrder(FName& OutOrderFood);

	int32 GetCurrentCustomerCount() const { return Customers.Num(); }

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<URSTycoonInventoryComponent> Inventory; //인벤토리

private:
	UPROPERTY(VisibleAnywhere)
	//오더 들어온 음식의 Key들
	TArray<FName> OrderedFoodKeys; 

	UPROPERTY()
	//가게 안에 있는 손님들
	TArray<TWeakObjectPtr<ARSTycoonCustomerCharacter>> Customers; 

	UPROPERTY(VisibleAnywhere)
	//최대 입장 가능한 손님, TableTile의 영향을 받음
	int32 MaxCustomerCount; 

	FTimerHandle CustomerTimerHandle; //손님 등장 타이머
};
