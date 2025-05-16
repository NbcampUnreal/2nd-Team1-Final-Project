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

	void Attach();

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> Camera;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> TopCamera;	//식당 관리 때 쓰일 탑뷰 카메라
};
