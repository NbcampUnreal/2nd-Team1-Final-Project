// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RSTycoonBaseAICharacter.generated.h"

class ARSTycoonBaseAIController;
class UBlackBoardComponent;
enum class EBaseTycoonAIState : uint8;

UCLASS()
class ROGSHOP_API ARSTycoonBaseAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARSTycoonBaseAICharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 타겟 위치로 이동
	UFUNCTION(BlueprintCallable, Category = "AI Movement")
	void MoveToTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 현재 AI 상태 (Walking, Working, Waiting 등)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI State")
	EBaseTycoonAIState CurrentAIState;

	// 목표 액터(테이블, 손님, 요리대) 등
	TObjectPtr<AActor> CurrentTarget;

private:
	// AI 컨트롤러 참조
	TObjectPtr<ARSTycoonBaseAIController> AIController;
	// 블랙보드 컴포넌트 참조
	TObjectPtr<UBlackBoardComponent> BlackBoardComp;

};
