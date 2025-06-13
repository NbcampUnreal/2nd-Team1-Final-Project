// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSBushTriggerSoundActor.generated.h"


class USoundBase;
class UCapsuleComponent;
class UStaticMeshComponent;


UCLASS()
class ROGSHOP_API ARSBushTriggerSoundActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARSBushTriggerSoundActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	); // 플레이어가 BushTriggerSoundActor의 범위에 들어왔을 때 호출되는 함수

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex); // 플레이어가 BushTriggerSoundActor의 범위에서 벗어났을 때 호출되는 함수

	UFUNCTION()
	void CheckMovementAndPlaySound(); //플레이어가 움직였는지 확인할 함수



public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCapsuleComponent> CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> BushMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<USoundBase> SoundToPlay;

private:
	FTimerHandle MovementCheckTimerHandle;

	FVector LastKnownPlayerLocation; // 마지막으로 확인한 플레이어 위치
	TWeakObjectPtr<ACharacter> OverlappingCharacter; // 현재 범위에 있는 플레이어 캐릭터에 대한 약한 참조


};
