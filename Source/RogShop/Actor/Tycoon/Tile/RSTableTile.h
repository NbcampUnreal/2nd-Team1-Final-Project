// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSBaseTile.h"
#include "RSTableTile.generated.h"

class ARSTycoonCustomerCharacter;

UCLASS()
class ROGSHOP_API ARSTableTile : public ARSBaseTile
{
	GENERATED_BODY()

public:
	ARSTableTile();

	GENERATED_TILE()
	
	virtual void Interact(ACharacter* InteractCharacter) override;
	void Sit(ARSTycoonCustomerCharacter* Customer);

	bool CanSit() const { return SittingCustomers.Num() < SittingLocations.Num(); }
	bool Use() const { return SittingCustomers.Num() > 0; }
	FVector GetFoodLocation() const { return FoodLocation->GetComponentLocation(); }
	int32 GetMaxPlace() const { return SittingLocations.Num(); }
	FTransform GetSitTransform() const { return SittingLocations[SittingCustomers.Num()]->GetComponentTransform(); }
	ARSTycoonCustomerCharacter* GetMainCustomer() const {return SittingCustomers[0];}
	
private:
	void Order();
	void Serving(ACharacter* InteractCharacter);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
	TObjectPtr<USceneComponent> FoodLocation; //음식이 위치할 곳

	UPROPERTY()
	TObjectPtr<AActor> FoodActor; //배치된 음식
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<TObjectPtr<USceneComponent>> SittingLocations; //손님이 앉는 위치들

	UPROPERTY()
	TArray<TObjectPtr<ARSTycoonCustomerCharacter>> SittingCustomers; //앉아있는 손님들
};
