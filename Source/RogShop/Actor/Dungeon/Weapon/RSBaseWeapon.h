// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSInteractable.h"
#include "RSBaseWeapon.generated.h"

UCLASS()
class ROGSHOP_API ARSBaseWeapon : public AActor, public IRSInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARSBaseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

// 상호작용
public:
	virtual void Interact(ARSDunPlayerCharacter* Interactor) override;

// 스켈레탈 메시
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> SceneComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<USkeletalMeshComponent> MeshComp;

// 애니메이션
public:
	UAnimMontage* GetNormalAttack(int32 Index);
	const TArray<UAnimMontage*>& GetNormalAttacks();
		
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim", meta = (AllowPrivateAccess = true))
	TSubclassOf<UAnimInstance> AnimInstnace; // 무기의 기본 이동 애님 인스턴스

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim", meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UAnimMontage>> NormalAttacks; // 무기의 기본 공격 몽타주
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim", meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UAnimMontage>> StrongAttacks; // 무기의 강 공격 몽타주
};
