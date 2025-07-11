// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSDungeonItemBase.h"
#include "RSInteractable.h"
#include "RSBaseWeapon.generated.h"

class UBoxComponent;
class UNiagaraComponent;
class UNiagaraSystem;

struct FWeaponAttackData;

UCLASS()
class ROGSHOP_API ARSBaseWeapon : public ARSDungeonItemBase
{
	GENERATED_BODY()
	
public:	
	ARSBaseWeapon();

protected:
	virtual void BeginPlay() override;

// 컴포넌트
public:
	UBoxComponent* GetBoxComp() const;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> SceneComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BoxComp;	// 데미지 로직을 위한 콜리전 용도의 컴포넌트

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> TrailNiagaraComp;

// 애니메이션
public:
	TSubclassOf<UAnimInstance> GetWeaponAnimInstnace() const;
	
	UAnimMontage* GetNormalAttackMontage(int32 Index) const;

	const TArray<UAnimMontage*> GetNormalAttackMontages() const;

	UAnimMontage* GetStrongAttackMontage(int32 Index) const;

	const TArray<UAnimMontage*> GetStrongAttackMontages() const;
		
private:
	// 애니메이션들을 캐싱한다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anim", meta = (AllowPrivateAccess = "true"))
	TArray<FWeaponAttackData> NormalAttackDatas; // 무기의 기본 공격 몽타주

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anim", meta = (AllowPrivateAccess = "true"))
	TArray<FWeaponAttackData> StrongAttackDatas; // 무기의 강 공격 몽타주

// 충돌
public:
	// 트레이스 채널을 사용하기 때문에 더이상 사용하지 않는 기능
	void StartOverlap();
	// 트레이스 채널을 사용하기 때문에 더이상 사용하지 않는 기능
	void EndOverlap();

	float GetNormalAttackMontageDamage(int32 MontageIndex) const;

	float GetStrongAttackMontageDamage(int32 MontageIndex) const;

// 부착할 소켓
public:
	FName GetAttachSocketName() const;

private:
	// 부착할 소켓 이름
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AttachSocket", meta = (AllowPrivateAccess = "true"))
	FName AttachSocket;

// 나이아가라
public:
	void StartTrail();

	void EndTrail();

	void SpawnHitImpactEffect(FVector TargetLocation);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Niagara", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> TrailEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Niagara", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> HitImpactEffect;

// 데이터 테이블의 RowName을 ID값으로 사용한다.
public:
	FName GetDataTableKey() const;
	void SetDataTableKey(FName NewDataTableKey);

private:
	void Initialization();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info", meta = (AllowPrivateAccess = "true"))
	FName DataTableKey;
};
