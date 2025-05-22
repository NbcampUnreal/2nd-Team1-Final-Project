// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TycoonDatas.h"
#include "GameFramework/GameModeBase.h"
#include "RSTycoonGameModeBase.generated.h"

class ARSTycoonCustomerCharacter;
class URSTycoonInventoryComponent;

UCLASS()
class ROGSHOP_API ARSTycoonGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARSTycoonGameModeBase();

	void StartGame();
	void AddOrder(FFoodOrder Order);
	void RemoveOrder(FFoodOrder Order);
	void RemoveCustomer(ARSTycoonCustomerCharacter* Customer);

	const TArray<FFoodOrder>& GetOrders() const { return FoodOrders; }
	const TArray<TWeakObjectPtr<ARSTycoonCustomerCharacter>>& GetCustomers() const { return Customers; }

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
	//오더 들어온 음식의 Key들
	UPROPERTY(VisibleAnywhere)
	TArray<FFoodOrder> FoodOrders; 

	//가게 안에 있는 손님들
	UPROPERTY()
	TArray<TWeakObjectPtr<ARSTycoonCustomerCharacter>> Customers; 

	//최대 입장 가능한 손님, TableTile의 영향을 받음
	UPROPERTY(VisibleAnywhere)
	int32 MaxCustomerCount; 

	FTimerHandle CustomerTimerHandle; //손님 등장 타이머
	int32 Money;
};
