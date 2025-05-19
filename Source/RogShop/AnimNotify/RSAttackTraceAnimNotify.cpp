// Fill out your copyright notice in the Description page of Project Settings.


#include "RSAttackTraceAnimNotify.h"
#include "RSDunMonsterCharacter.h"

void URSAttackTraceAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UE_LOG(LogTemp, Warning, TEXT("Notify Success!!"));
	if (MeshComp && MeshComp->GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("Notify GetOwner!!"));
		AActor* Owner = MeshComp->GetOwner();
		ARSDunMonsterCharacter* DunMonsterCharacter = Cast<ARSDunMonsterCharacter>(Owner);

		if (DunMonsterCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("Notify DunMonsterCharacter Nice Casting!!"));
			DunMonsterCharacter->PerformAttackTrace();
		}
	}
}
