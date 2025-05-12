// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RSDunBaseCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API ARSDunBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARSDunBaseCharacter();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float WalkSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Stat", meta = (AllowPrivateAccess = "true"))
	float MaxHP;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, category = "Stat", meta = (AllowPrivateAccess = "true"))
	float HP;
};
