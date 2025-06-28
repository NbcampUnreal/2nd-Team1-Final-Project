// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TycoonDatas.h"
#include "GameFramework/GameModeBase.h"
#include "RSTycoonGameModeBase.generated.h"

class URSTycoonEventViewWidget;
class URSTycoonEvent;
class ARSTycoonNPC;
class ARSTycoonCustomerCharacter;
class URSTycoonInventoryComponent;

UENUM()
enum class ETycoonGameMode : uint8
{
	None,
	Wait,
	Sale,
	EndSale,
	Management
};

UCLASS()
class ROGSHOP_API ARSTycoonGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARSTycoonGameModeBase();

	void StartSaleMode();
	
	void SetMaxCustomerCount();
	
	void AddOrder(FFoodOrder Order);
	
	void RemoveOrder(FFoodOrder Order);
	
	void RemoveCustomer(ARSTycoonCustomerCharacter* Customer);
	
	void AddNPC(ARSTycoonNPC* NPC);

	void RemoveNPC(ARSTycoonNPC* NPC);
	
	void EndSaleMode();
	
	void StartWaitMode();
	
	void StartManagementMode();

	//남은 재료중에 요리가 가능한지 반환
	//OutOrderFood : true로 통과된다면 해야할 요리
	UFUNCTION(BlueprintCallable)
	bool CanOrder(FName& OutOrderFood);

	
	float GetGameTime() const; //게임 경과 시간을 반환해줌
	const TArray<FFoodOrder>& GetOrders() const { return FoodOrders; }
	FFoodOrder GetOrderToCook();
	const TArray<TObjectPtr<ARSTycoonCustomerCharacter>>& GetCustomers() const { return Customers; }
	const TArray<ARSTycoonNPC*>& GetNPCs() const { return NPCs; }
	int32 GetSaleStartHour() const { return SaleStartHour; }
	int32 GetSaleHourDuration() const { return SaleEndHour - SaleStartHour; }
	float GetSaleRealPlayMinute() const { return SalePlayMinute; }
	ETycoonGameMode GetState() const { return State; }

protected:
	virtual void BeginPlay() override;

private:
	void CreateCustomer();
	void LoadGameData();

	UFUNCTION()
	void SaveGameData();

	int32 GetCurrentCustomerCount() const { return Customers.Num(); }

private:
	//오더 들어온 음식의 Key들
	UPROPERTY(VisibleAnywhere)
	TArray<FFoodOrder> FoodOrders;

	//가게 안에 있는 손님들
	UPROPERTY()
	TArray<TObjectPtr<ARSTycoonCustomerCharacter>> Customers;

	//최대 입장 가능한 손님, TableTile의 영향을 받음
	UPROPERTY(VisibleAnywhere)
	int32 MaxCustomerCount;

	//가게 안에 있는 NPC들 (※ 손님 제외)
	UPROPERTY()
	TArray<ARSTycoonNPC*> NPCs;

	UPROPERTY(EditDefaultsOnly, meta=(ClampMin = "0", ClampMax = "24", UIMin = "0", UIMax = "24"))
	int32 SaleStartHour = 10;

	UPROPERTY(EditDefaultsOnly, meta=(ClampMin = "0", ClampMax = "24", UIMin = "0", UIMax = "24"))
	int32 SaleEndHour = 18;

	UPROPERTY(EditDefaultsOnly)
	float SalePlayMinute = 5; //게임 플레이 시간, 기본 5분
	
	// 사운드
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> CustomerAddSound;
	
	ETycoonGameMode State; //현재 레벨이 어떤 모드인가
	FTimerHandle CustomerTimerHandle; //손님 등장 타이머
	FTimerHandle GameTimerHandle; //게임 경과 타이머
};
