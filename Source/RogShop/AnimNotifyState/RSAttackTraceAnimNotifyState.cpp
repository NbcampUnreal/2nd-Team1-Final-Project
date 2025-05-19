// Fill out your copyright notice in the Description page of Project Settings.


#include "RSAttackTraceAnimNotifyState.h"
#include "RSDunMonsterCharacter.h"

void URSAttackTraceAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	UE_LOG(LogTemp, Warning, TEXT("Notify Begin"));
}

void URSAttackTraceAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("Notify Success1!!"));

	if (MeshComp && MeshComp->GetOwner())
	{
		AActor* Owner = MeshComp->GetOwner();
		auto DunMonsterCharacter = Cast<ARSDunMonsterCharacter>(Owner);

		UE_LOG(LogTemp, Warning, TEXT("Notify Success2!!"));

		//if (DunMonsterCharacter && !DunMonsterCharacter->HasAttackTraced())

		if (DunMonsterCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("Notify Success3!!"));
			DunMonsterCharacter->PerformAttackTrace();
			//DunMonsterCharacter->SetAttackTraced(true);
		}
	}
}

void URSAttackTraceAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	UE_LOG(LogTemp, Warning, TEXT("Notify End"));

	if (MeshComp && MeshComp->GetOwner())
	{
		auto DunMonsterCharacter = Cast<ARSDunMonsterCharacter>(MeshComp->GetOwner());
		if (DunMonsterCharacter)
		{
			DunMonsterCharacter->SetAttackTraced(false);
		}
	}
}
