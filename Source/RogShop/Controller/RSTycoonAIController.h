// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RSTycoonAIController.generated.h"

class UBehaviorTreeComponent;
class UBlackBoardComponent;

struct FAIStimulus;

UCLASS()
class ROGSHOP_API ARSTycoonAIController : public AAIController
{
	GENERATED_BODY()

public:
	ARSTycoonAIController();

    UPROPERTY(BlueprintReadWrite, Category = "Tycoon AI")
    TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

    UPROPERTY(BlueprintReadWrite, Category = "Tycoon AI")
    TObjectPtr<UBlackboardComponent> BlackboardComponent;

    UPROPERTY(EditAnywhere, Category = "Tycoon AI")
    TObjectPtr<UBehaviorTree> BehvaiorTree;
	
protected:
    virtual void OnPossess(APawn* InPawn) override;

    // Ÿ���� ��ġ ������ ��� ������Ʈ�ϰ� �޾ƿ��� �Լ�
    void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

private:
    // ������ Ű �̸� ���
    static const FName TargetActorKey;

};
