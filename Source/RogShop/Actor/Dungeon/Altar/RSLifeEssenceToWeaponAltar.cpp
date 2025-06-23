// Fill out your copyright notice in the Description page of Project Settings.


#include "RSLifeEssenceToWeaponAltar.h"
#include "RSDunPlayerCharacter.h"
#include "GameFramework/GameModeBase.h"
#include "RSSpawnManagerAccessor.h"
#include "RSAltarCostWidget.h"

ARSLifeEssenceToWeaponAltar::ARSLifeEssenceToWeaponAltar()
{
	InteractName = FText::FromString(TEXT("무기의 제단"));

	Cost = 25;
}

void ARSLifeEssenceToWeaponAltar::BeginPlay()
{
	Super::BeginPlay();

	URSAltarCostWidget* CostWidget = Cast<URSAltarCostWidget>(GetCostWidgetObject());
	if (CostWidget)
	{
		CostWidget->ChangeColor(FLinearColor::Yellow);
		CostWidget->UpdateCost(FString::FromInt(Cost));
	}
}

void ARSLifeEssenceToWeaponAltar::Interact(ARSDunPlayerCharacter* Interactor)
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

	// 드랍 가능한 무작위 무기를 하나 드랍한다.
	IRSSpawnManagerAccessor* SpawnManagerAccessor = GetWorld()->GetAuthGameMode<IRSSpawnManagerAccessor>();
	URSSpawnManager* SpawnManager = nullptr;
	if (SpawnManagerAccessor)
	{
		SpawnManager = SpawnManagerAccessor->GetSpawnManager();

	}

	if (SpawnManager)
	{
		TArray<FName> UnspawnedWeapons = SpawnManager->GetUnspawnedWeapons().Array();
		int32 SpawnIndex = FMath::RandRange(0, UnspawnedWeapons.Num() - 1);
		FName SpawnName = UnspawnedWeapons[SpawnIndex];

		SpawnManager->SpawnGroundWeaponAtTransform(SpawnName, GetActorTransform(), true);

		// 비용을 2배로 증가시킨다.
		Cost *= 2;
	}

	// 비용 업데이트
	URSAltarCostWidget* CostWidget = Cast<URSAltarCostWidget>(GetCostWidgetObject());
	if (CostWidget)
	{
		CostWidget->UpdateCost(FString::FromInt(Cost));
	}
}
