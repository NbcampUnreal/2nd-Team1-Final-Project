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

	virtual void StopAllAction() override;
	
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ETycoonCustomerState State;
	
	// 애님 몽타주
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage")
	TObjectPtr<UAnimMontage> SitMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage")
	TObjectPtr<UAnimMontage> StandMontage;
	
private:
	UPROPERTY()
	TObjectPtr<ARSTableTile> SitTableTile;

	// 스켈레탈 메시 관련
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SkeletalMesh", meta = (AllowPrivateAccess = true))
	FSkeletalMeshMergeParams SkeletalMeshMergeParams;

	// 사운드
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> SitSound;
};
