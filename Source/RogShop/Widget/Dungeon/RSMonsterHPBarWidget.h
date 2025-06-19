// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSMonsterHPBarWidget.generated.h"

class UProgressBar;

UCLASS()
class ROGSHOP_API URSMonsterHPBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void UpdateTargetPercent(const float NewPercent);

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* MonsterHPBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* MonsterHPBackBar;

private:
	float TargetPercent;
	float CurrentPercent;
	float InterpSpeed;

};

