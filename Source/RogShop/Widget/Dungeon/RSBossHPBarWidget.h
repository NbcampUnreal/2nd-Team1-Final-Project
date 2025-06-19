// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSMonsterHPBarWidget.h"
#include "RSBossHPBarWidget.generated.h"

class UTextBlock;
class UProgressBar;

UCLASS()
class ROGSHOP_API URSBossHPBarWidget : public URSMonsterHPBarWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void UpdateBossHPBar(FName BossName, float HPPercent);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> BossNameTextBlock;

};
