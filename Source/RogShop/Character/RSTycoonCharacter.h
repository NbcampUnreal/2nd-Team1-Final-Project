// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RSTycoonCharacter.generated.h"

class USphereComponent;
struct FInputActionValue;

UCLASS()
class ROGSHOP_API ARSTycoonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARSTycoonCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	virtual void BeginPlay() override;

private:
	void OnMove(const FInputActionValue& Value);
	void OnInteract(const FInputActionValue& Value);

	UFUNCTION()
	void OnBeginOverlapInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> InteractSphere;
};
