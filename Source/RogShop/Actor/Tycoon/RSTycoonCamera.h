// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSTycoonCamera.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class ROGSHOP_API ARSTycoonCamera : public AActor
{
	GENERATED_BODY()

public:
	ARSTycoonCamera();

	void AttachPlayer();
	void SetLocationToCenter();

	TObjectPtr<UCameraComponent> GetCameraComponent() const { return Camera; }

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> Camera;
};
