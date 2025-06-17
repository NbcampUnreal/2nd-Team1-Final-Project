// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RSTycoonNPC.generated.h"

class USphereComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInteractTarget, AActor*)

UCLASS()
class ROGSHOP_API ARSTycoonNPC : public ACharacter
{
	GENERATED_BODY()

public:
	ARSTycoonNPC();
	
	UFUNCTION(BlueprintCallable)
	void MoveToTarget(FVector Location, AActor* Target);
	
	virtual void StopAllAction();
	
	bool HasTarget() const { return MoveTarget != nullptr; }
	FString GetDisplayName() const { return DisplayName; }
	int32 GetPrice() const { return Price; }
	TObjectPtr<UTexture2D> GetThumbnail() const { return Thumbnail; }
	
protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	
	virtual void InteractTarget(AActor* TargetActor);
	
private:
	UFUNCTION()
	void OnInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
public:
	FOnInteractTarget OnInteractTarget;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USphereComponent> InteractSphere;

	UPROPERTY()
	TObjectPtr<AActor> MoveTarget;

	UPROPERTY(EditAnywhere, Category="Information")
	FString DisplayName = TEXT("NPC");
	
	UPROPERTY(EditAnywhere, Category="Information")
	int32 Price = 10;

	UPROPERTY(EditAnywhere, Category="Information")
	TObjectPtr<UTexture2D> Thumbnail;

private:
	FTimerHandle MoveTargetTimer;
};
