// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSInteractable.h"
#include "RSBaseTreasureChest.generated.h"

class UNiagaraComponent;
class UAnimMontage;

UENUM(BlueprintType)
enum class ETreasureType : uint8
{
	Weapon,
	Relic,
	Gold
};

UCLASS()
class ROGSHOP_API ARSBaseTreasureChest : public AActor, public IRSInteractable
{
	GENERATED_BODY()

public:
	ARSBaseTreasureChest();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Interact(ARSDunPlayerCharacter* Interactor) override;

	virtual FText GetInteractName() const override;

	virtual bool GetIsAutoInteract() const override;

	virtual UAnimMontage* GetInteractAnimMontage() const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact")
	FText InteractName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interact")
	bool bIsAutoInteract;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interact")
	bool bIsOpened;

	// 상호작용 애니메이션 몽타주
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim")
	TObjectPtr<UAnimMontage> InteractAnimMontage;

	// 상자 보상 종류
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reward")
	ETreasureType TreasureType;

	// 기본 정보
public:
	void InitTreasureInfo(FText NewInteractName, bool bNewIsAutoInteract, FName NewDataTableKey, USkeletalMesh* NewMesh);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	FName DataTableKey;	// 데이터 테이블의 RowName을 ID값으로 사용한다.

	// 컴포넌트
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> SceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<USkeletalMeshComponent> MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = true))
	TObjectPtr<UNiagaraComponent> OpenEffectComp;
};
