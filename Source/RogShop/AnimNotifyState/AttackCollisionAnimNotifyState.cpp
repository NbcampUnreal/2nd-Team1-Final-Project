// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackCollisionAnimNotifyState.h"
#include "RSDunMonsterCharacter.h"
#include "MeleeAttackBoxComponent.h"

void UAttackCollisionAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	auto Monster = Cast<ARSDunMonsterCharacter>(MeshComp->GetOwner());
	if (Monster)
	{
		UMeleeAttackBoxComponent* MeeleAttackBoxComp = Monster->MeleeAttackBoxComponent;
		if (MeeleAttackBoxComp)
		{
			MeeleAttackBoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			UE_LOG(LogTemp, Warning, TEXT("SetCollisionEnabled QueryOnly Success"));

			DrawDebugBox(
				MeshComp->GetWorld(),
				MeeleAttackBoxComp->GetComponentLocation(),
				MeeleAttackBoxComp->GetScaledBoxExtent(),
				MeeleAttackBoxComp->GetComponentQuat(),
				FColor::Red,
				false,         // true: 시간 지나도 계속 보임
				10.0f,         // 10초 동안 유지
				0,
				2.0f           // 두께
			);
		}
	}
}

void UAttackCollisionAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	auto Monster = Cast<ARSDunMonsterCharacter>(MeshComp->GetOwner());
	if (Monster)
	{
		UMeleeAttackBoxComponent* MeeleAttackBoxComp = Monster->MeleeAttackBoxComponent;
		if (MeeleAttackBoxComp)
		{
			MeeleAttackBoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			UE_LOG(LogTemp, Warning, TEXT("SetCollisionEnabled NoCollision Success"));
		}
	}
}
