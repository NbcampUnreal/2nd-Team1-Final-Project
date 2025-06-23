// Fill out your copyright notice in the Description page of Project Settings.


#include "RSLifeEssenceToConversionAltar.h"
#include "RSDunPlayerCharacter.h"
#include "GameFramework/GameModeBase.h"
#include "RSSpawnManagerAccessor.h"
#include "RSAltarCostWidget.h"

ARSLifeEssenceToConversionAltar::ARSLifeEssenceToConversionAltar()
{
	InteractName = FText::FromString(TEXT("보물의 제단"));

	Cost = 25;
}

void ARSLifeEssenceToConversionAltar::BeginPlay()
{
	Super::BeginPlay();

	URSAltarCostWidget* CostWidget = Cast<URSAltarCostWidget>(GetCostWidgetObject());
	if (CostWidget)
	{
		CostWidget->ChangeColor(FLinearColor::Yellow);
		CostWidget->UpdateCost(FString::FromInt(Cost));
	}
}

void ARSLifeEssenceToConversionAltar::Interact(ARSDunPlayerCharacter* Interactor)
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
		TArray<FName> UnspawnedRelics = SpawnManager->GetUnspawnedRelics().Array();

		bool bChooseWeapon = FMath::RandBool();

		if (bChooseWeapon && UnspawnedWeapons.Num() > 0)
		{
			int32 WeaponSpawnIndex = FMath::RandRange(0, UnspawnedWeapons.Num() - 1);
			FName SpawnName = UnspawnedWeapons[WeaponSpawnIndex];

			SpawnManager->SpawnGroundWeaponAtTransform(SpawnName, GetActorTransform(), true);
		}
		else if (UnspawnedRelics.Num() > 0)
		{
			int32 RelicSpawnIndex = FMath::RandRange(0, UnspawnedRelics.Num() - 1);
			FName SpawnName = UnspawnedRelics[RelicSpawnIndex];

			SpawnManager->SpawnGroundRelicAtTransform(SpawnName, GetActorTransform());
		}
		else if (UnspawnedWeapons.Num() > 0)
		{
			int32 WeaponSpawnIndex = FMath::RandRange(0, UnspawnedWeapons.Num() - 1);
			FName SpawnName = UnspawnedWeapons[WeaponSpawnIndex];

			SpawnManager->SpawnGroundWeaponAtTransform(SpawnName, GetActorTransform(), true);
		}

		// 비용을 2배로 증가시킨다.
		Cost *= 2;

		// 만약 남은 무기나 유물이 없는 경우 비용을 0으로 설정합니다.
		if (UnspawnedWeapons.Num() == 1 && UnspawnedRelics.Num() == 0 ||
			UnspawnedWeapons.Num() == 0 && UnspawnedRelics.Num() == 1)
		{
			Cost = 0;

		}
	}

	// 비용 업데이트
	URSAltarCostWidget* CostWidget = Cast<URSAltarCostWidget>(GetCostWidgetObject());
	if (CostWidget)
	{
		CostWidget->UpdateCost(FString::FromInt(Cost));
	}
}
