// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSTycoonNPC.h"
#include "RSTycoonChefCharacter.generated.h"

class UWidgetComponent;
class ARSCookingTile;
class ARSTileMap;

UCLASS()
class ROGSHOP_API ARSTycoonChefCharacter : public ARSTycoonNPC
{
	GENERATED_BODY()

public:
	ARSTycoonChefCharacter();

	virtual void Tick(float DeltaSeconds) override;
	virtual void StopAllAction() override;
	
	void FindCookingTile();
	void LeaveCookingTile();

protected:
	virtual void BeginPlay() override;
	virtual void InteractTarget(AActor* TargetActor) override;

private:
	void TryCook();
	void CheckFinish();

protected:
	UPROPERTY(EditDefaultsOnly)	
	TObjectPtr<UWidgetComponent> FoodBubbleWidgetComponent;
	
	UPROPERTY(BlueprintReadOnly)
	bool bCooking;
	
private:
	UPROPERTY()
	TObjectPtr<ARSCookingTile> PlacedCookingTile; //배치된 쿠킹 타일

	// 사운드
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> CookingSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> CookingFinishSound;

	UPROPERTY()
	TObjectPtr<UAudioComponent> CookingAudioComponent;
};
