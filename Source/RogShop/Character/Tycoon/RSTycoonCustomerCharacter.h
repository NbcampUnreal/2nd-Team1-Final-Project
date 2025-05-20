// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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
class ROGSHOP_API ARSTycoonCustomerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARSTycoonCustomerCharacter();

	UFUNCTION(BlueprintCallable)
	void MoveToTarget(FVector Location, AActor* Target);

	void Sit(const FTransform& SitTransform);
	void WaitFood();
	void Eat();
	
protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
public:
	FName WantFoodKey;
	
	UPROPERTY(VisibleAnywhere)
	ETycoonCustomerState State;

	FOnLeave OnLeave;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> InteractSphere;

	UPROPERTY()
	TWeakObjectPtr<AActor> MoveTarget;
};
