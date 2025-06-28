// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RSGameInstance.generated.h"

UCLASS()
class ROGSHOP_API URSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// 로그 입출력 관리
	bool GetDebugLogEnabled() const;
	void SetDebugLogEnabled(bool bEnable);

	// 구매한 아이템의 아이디 저장
	TArray<FName> PurchasedItemIDs;

private:
	bool bEnableDebugLog = false;

public:
	UFUNCTION()
	void OnMasterVolumeChanged(float Value);

	UFUNCTION()
	void OnBGMVolumeChanged(float Value);

	UFUNCTION()
	void OnSFXVolumeChanged(float Value);
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundClass* SC_Master;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundClass* SC_BGM;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundClass* SC_SFX;
};
