// Fill out your copyright notice in the Description page of Project Settings.


#include "RSHPToLifeEssenceAltar.h"
#include "RSDunPlayerCharacter.h"
#include "GameFramework/GameModeBase.h"
#include "RSSpawnManagerAccessor.h"
#include "RSAltarCostWidget.h"

ARSHPToLifeEssenceAltar::ARSHPToLifeEssenceAltar()
{
	InteractName = FText::FromString(TEXT("핏값의 제단"));

	Cost = 10;

}

void ARSHPToLifeEssenceAltar::BeginPlay()
{
	Super::BeginPlay();

	URSAltarCostWidget* CostWidget = Cast<URSAltarCostWidget>(GetCostWidgetObject());
	if (CostWidget)
	{
		CostWidget->ChangeColor(FLinearColor::Red);
		CostWidget->UpdateCost(FString::FromInt(Cost) + "%");
	}
}

void ARSHPToLifeEssenceAltar::Interact(ARSDunPlayerCharacter* Interactor)
{
	if (!Interactor)
	{
		return;
	}

	// 깎으려는 HP를 구한다.
	// 최대 체력의 10퍼센트를 구하며, 내림한다.
	float TargetMaxHP = Interactor->GetMaxHP();
	int32 HPCost = TargetMaxHP * Cost / 100;

	Interactor->DecreaseHP(HPCost);

	// 깎은 채력 만큼의 던전 재화를 드랍한다.
	IRSSpawnManagerAccessor* SpawnManagerAccessor = GetWorld()->GetAuthGameMode<IRSSpawnManagerAccessor>();
	URSSpawnManager* SpawnManager = nullptr;
	if (SpawnManagerAccessor)
	{
		SpawnManager = SpawnManagerAccessor->GetSpawnManager();
		
	}

	for (int32 i = 0; i < HPCost; ++i)
	{
		FTimerHandle SpawnDelayTimerHandle;

		GetWorld()->GetTimerManager().SetTimer(SpawnDelayTimerHandle, FTimerDelegate::CreateLambda([=, this]()
			{
				if (IsValid(this) && IsValid(SpawnManager))
				{
					SpawnManager->SpawnGroundLifeEssenceAtTransform(GetActorTransform(), 1);
				}

			}),
			i * 0.05f,
			false);
	}

	// 비용을 2배로 증가시킨다.
	Cost *= 2;

	// 비용 업데이트
	URSAltarCostWidget* CostWidget = Cast<URSAltarCostWidget>(GetCostWidgetObject());
	if (CostWidget)
	{
		CostWidget->UpdateCost(FString::FromInt(Cost) + "%");
	}
}
