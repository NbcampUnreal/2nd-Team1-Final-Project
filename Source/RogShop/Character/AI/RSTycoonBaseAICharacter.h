// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RSTycoonBaseAICharacter.generated.h"

class ARSTycoonBaseAIController;
class UBlackBoardComponent;
enum class EBaseTycoonAIState;

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

	// Ÿ�� ��ġ�� �̵�
	UFUNCTION(BlueprintCallable, Category = "AI Movement")
	void MoveToTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// ���� AI ���� (Walking, Working, Waiting ��)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI State")
	EBaseTycoonAIState CurrentAIState;

	// ��ǥ ����(���̺�, �մ�, �丮��) ��
	TObjectPtr<AActor> CurrentTarget;

private:
	// AI ��Ʈ�ѷ� ����
	TObjectPtr<ARSTycoonBaseAIController> AIController;
	// ������ ������Ʈ ����
	TObjectPtr<UBlackBoardComponent> BlackBoardComp;

};
