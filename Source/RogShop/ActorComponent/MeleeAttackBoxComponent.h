// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "MeleeAttackBoxComponent.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API UMeleeAttackBoxComponent : public UBoxComponent
{
	GENERATED_BODY()
	
public:
	UMeleeAttackBoxComponent();

protected:
	virtual void BeginPlay() override;

	// ���� ������ �̺�Ʈ ó�� �Լ�
	UFUNCTION()
	void OnAttackOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);


};
