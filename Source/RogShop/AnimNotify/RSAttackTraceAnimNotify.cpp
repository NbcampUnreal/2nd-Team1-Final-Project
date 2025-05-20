// Fill out your copyright notice in the Description page of Project Settings.


#include "RSAttackTraceAnimNotify.h"
#include "RSDunMonsterCharacter.h"

void URSAttackTraceAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UE_LOG(LogTemp, Warning, TEXT("Notify Success!!"));
	AActor* Owner = MeshComp->GetOwner();

	if (MeshComp && Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("Notify GetOwner!!"));
		ARSDunMonsterCharacter* DunMonsterCharacter = Cast<ARSDunMonsterCharacter>(Owner);

		if (DunMonsterCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("Notify DunMonsterCharacter Nice Casting!!"));
			DunMonsterCharacter->PerformAttackTrace();
		}
	}
}
