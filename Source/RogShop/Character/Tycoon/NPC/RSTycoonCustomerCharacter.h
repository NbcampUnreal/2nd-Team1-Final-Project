// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "RSTycoonNPC.h"
#include "SkeletalMergingLibrary.h"
#include "RSTycoonCustomerCharacter.generated.h"

class UWidgetComponent;
class ARSTableTile;
class USphereComponent;
class ARSTycoonBaseAIController;
class UBlackBoardComponent;

UENUM(BlueprintType)
enum class ETycoonCustomerState : uint8
{
	Move = 0 UMETA(DisplayName = "Move"),
	OrderWaiting = 1 UMETA(DisplayName = "Order Waiting"),
	FoodWaiting = 2 UMETA(DisplayName = "Food Waiting"),
	Eat = 3 UMETA(DisplayName = "Eat"),
};

class ARSTycoonCustomerCharacter;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnFinishEat, ARSTycoonCustomerCharacter*);

UCLASS()
class ROGSHOP_API ARSTycoonCustomerCharacter : public ARSTycoonNPC
{
	GENERATED_BODY()

public:
	ARSTycoonCustomerCharacter();

	void Sit(ARSTableTile* Table, const FTransform& SitTransform);
	void WaitFood();
	void Eat();

	ETycoonCustomerState GetState() const { return State; }
	ARSTableTile* GetSittingTable() const { return SitTableTile; }

protected:
	virtual void BeginPlay() override;
	virtual void InteractTarget(AActor* TargetActor) override;
	
private:
	void Leave();
	
public:
	FName WantFoodKey;

	FOnFinishEat OnFinishEat;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UWidgetComponent> FoodBubbleWidgetComponent;

private:
	UPROPERTY(VisibleAnywhere)
	ETycoonCustomerState State;

	UPROPERTY()
	TObjectPtr<ARSTableTile> SitTableTile;

	// 스켈레탈 메시 관련
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SkeletalMesh", meta = (AllowPrivateAccess = true))
	FSkeletalMeshMergeParams SkeletalMeshMergeParams;
};
