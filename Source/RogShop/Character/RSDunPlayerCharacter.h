// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSDunBaseCharacter.h"
#include "RSDunPlayerCharacter.generated.h"

UCLASS()
class ROGSHOP_API ARSDunPlayerCharacter : public ARSDunBaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARSDunPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


};
