// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "RSTycoonNPC.h"
#include "RSTycoonCustomerCharacter.generated.h"

class USphereComponent;
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

class ARSTycoonCustomerCharacter;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLeave, ARSTycoonCustomerCharacter*);

UCLASS()
class ROGSHOP_API ARSTycoonCustomerCharacter : public ARSTycoonNPC
{
	GENERATED_BODY()

public:
	ARSTycoonCustomerCharacter();

	void Sit(const FTransform& SitTransform);
	void WaitFood();
	void Eat();

protected:
	virtual void InteractTarget(AActor* TargetActor) override;
	
public:
	FName WantFoodKey;
	
	UPROPERTY(VisibleAnywhere)
	ETycoonCustomerState State;

	FOnLeave OnLeave;
};
