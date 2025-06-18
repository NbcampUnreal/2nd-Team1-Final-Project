// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSMonsterHPBarWidget.h"
#include "RSBossHPBarWidget.generated.h"

class UTextBlock;

UCLASS()
class ROGSHOP_API URSBossHPBarWidget : public URSMonsterHPBarWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetBossName(const FName& BossNameInput);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* BossNameTextBlock;
	
};
