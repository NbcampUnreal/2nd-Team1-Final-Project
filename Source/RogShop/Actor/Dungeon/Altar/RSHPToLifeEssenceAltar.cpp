// Fill out your copyright notice in the Description page of Project Settings.


#include "RSHPToLifeEssenceAltar.h"
#include "RSDunPlayerCharacter.h"
#include "GameFramework/GameModeBase.h"
#include "RSSpawnManagerAccessor.h"

ARSHPToLifeEssenceAltar::ARSHPToLifeEssenceAltar()
{
	InteractName = FText::FromString(TEXT("피의 제단"));

	Cost = 10;
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

	if (SpawnManager)
	{
		SpawnManager->SpawnGroundLifeEssenceAtTransform(GetActorTransform(), HPCost);

		// 비용을 2배로 증가시킨다.
		Cost *= 2;
	}
	


}
