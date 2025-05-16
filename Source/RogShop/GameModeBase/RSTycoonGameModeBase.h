// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RSTycoonGameModeBase.generated.h"

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

	const TArray<FName>& GetOrders() const { return OrderedFoodKeys; }

protected:
	virtual void BeginPlay() override;

private:
	void CreateCustomer();
	bool CanOrder(FName& OutOrderFood);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<URSTycoonInventoryComponent> Inventory;	//인벤토리
	
private:
	UPROPERTY(VisibleAnywhere)
	TArray<FName> OrderedFoodKeys;		//오더 들어온 음식의 Key들

	UPROPERTY(VisibleAnywhere)
	int32 NowCustomerCount; 			//지금 가게에 있는 손님

	UPROPERTY(VisibleAnywhere)
	int32 MaxCustomerCount;				//최대 입장 가능한 손님, TableTile의 영향을 받음
	
	FTimerHandle CustomerTimerHandle;	//손님 등장 타이머
}; 
