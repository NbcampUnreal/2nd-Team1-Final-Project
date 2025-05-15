// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RSTycoonCustomerCharacter.generated.h"

class ARSTycoonBaseAIController;
class UBlackBoardComponent;

UENUM(BlueprintType)
enum class ETycoonCustomerState : uint8
{
	Move = 0            UMETA(DisplayName = "Move"),
	OrderWaiting = 1    UMETA(DisplayName = "Order Waiting"),
	FoodWaiting = 2     UMETA(DisplayName = "Food Waiting"),
	Eat = 3             UMETA(DisplayName = "Eat"),
};


UCLASS()
class ROGSHOP_API ARSTycoonCustomerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARSTycoonCustomerCharacter();

	UFUNCTION(BlueprintCallable, Category = "AI Movement")
	void MoveToTarget();


protected:
	virtual void BeginPlay() override;

public:
	FName WantFoodKey;
	
	// 현재 AI 상태
	UPROPERTY(VisibleAnywhere)
	ETycoonCustomerState State;

private:
	// 목표 위치
	FVector TargetLocation;

	// 블랙보드 컴포넌트 참조
	TObjectPtr<UBlackBoardComponent> BlackBoardComp;
};
