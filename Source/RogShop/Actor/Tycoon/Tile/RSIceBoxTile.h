// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSBaseTile.h"
#include "RSIceBoxTile.generated.h"

UCLASS()
class ROGSHOP_API ARSIceBoxTile : public ARSBaseTile
{
	GENERATED_BODY()

public:
	ARSIceBoxTile();
	
	GENERATED_TILE()
	
	virtual void Interact(ACharacter* InteractCharacter) override;

public:
	static const int32 AddInventorySlot;

private:
	UPROPERTY(EditDefaultsOnly, Category="Sound")
	TObjectPtr<USoundBase> OpenSound;
	
	UPROPERTY(EditDefaultsOnly, Category="Sound")
	TObjectPtr<USoundBase> CloseSound;
};
