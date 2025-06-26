// Fill out your copyright notice in the Description page of Project Settings.


#include "RSLifeEssenceToHPAltar.h"
#include "RSDunPlayerCharacter.h"
#include "GameFramework/GameModeBase.h"
#include "RSSpawnManagerAccessor.h"
#include "RSAltarCostWidget.h"

ARSLifeEssenceToHPAltar::ARSLifeEssenceToHPAltar()
{
	InteractName = FText::FromString(TEXT("식복의 제단"));

	Cost = 10;
}

void ARSLifeEssenceToHPAltar::BeginPlay()
{
	Super::BeginPlay();

	URSAltarCostWidget* CostWidget = Cast<URSAltarCostWidget>(GetCostWidgetObject());
	if (CostWidget)
	{
		CostWidget->ChangeColor(FLinearColor::Green);
		CostWidget->UpdateCost(FString::FromInt(Cost));
	}
}

void ARSLifeEssenceToHPAltar::Interact(ARSDunPlayerCharacter* Interactor)
{
	if (!Interactor)
	{
		return;
	}

	// 차감할 돈을 계산한다.
	int32 LifeEssenceAmount = Interactor->GetLifeEssence();

	// 부족한 경우 실행 취소
	if (0 > LifeEssenceAmount - Cost)
	{
		return;
	}

	Interactor->DecreaseLifeEssence(Cost);

	Interactor->IncreaseHP(10);

	// 비용을 5씩 증가시킨다.
	Cost += 5;

	// 비용 업데이트
	URSAltarCostWidget* CostWidget = Cast<URSAltarCostWidget>(GetCostWidgetObject());
	if (CostWidget)
	{
		CostWidget->UpdateCost(FString::FromInt(Cost));
	}
}
