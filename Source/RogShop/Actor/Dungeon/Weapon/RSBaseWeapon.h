// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSDungeonItemBase.h"
#include "RSInteractable.h"
#include "RSBaseWeapon.generated.h"

class UBoxComponent;

struct FWeaponAttackData;

UCLASS()
class ROGSHOP_API ARSBaseWeapon : public ARSDungeonItemBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARSBaseWeapon();

protected:
	// Called when the game starts or when spawned
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

// 애니메이션
public:
	TSubclassOf<UAnimInstance> GetWeaponAnimInstnace() const;
	
	UAnimMontage* GetNormalAttackMontage(int32 Index) const;

	const TArray<UAnimMontage*> GetNormalAttackMontages() const;

	void SetNormalAttackDatas(TArray<FWeaponAttackData> NewNormalAttackDatas);

	UAnimMontage* GetStrongAttackMontage(int32 Index) const;

	const TArray<UAnimMontage*> GetStrongAttackMontages() const;

	void SetStrongAttackDatas(TArray<FWeaponAttackData> NewStrongAttackDatas);
		
private:
	// 애니메이션들을 캐싱한다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UAnimInstance> WeaponAnimInstnace; // 무기의 기본 이동 애님 인스턴스

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

	float GetWeaponDamage() const;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float WeaponDamage;	// 오버랩 시 가할 무기 데미지

// 데이터 테이블의 RowName을 ID값으로 사용한다.
public:
	FName GetDataTableKey() const;
	void SetDataTableKey(FName NewDataTableKey);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info", meta = (AllowPrivateAccess = "true"))
	FName DataTableKey;
};
