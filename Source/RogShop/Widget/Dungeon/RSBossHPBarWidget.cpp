// Fill out your copyright notice in the Description page of Project Settings.

#include "RSBossHPBarWidget.h"
#include "Components/TextBlock.h"
#include "RSDunMonsterCharacter.h"

void URSBossHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void URSBossHPBarWidget::SetBossName(const FName& BossNameInput)
{
	BossNameTextBlock->SetText(FText::FromName(BossNameInput));

}