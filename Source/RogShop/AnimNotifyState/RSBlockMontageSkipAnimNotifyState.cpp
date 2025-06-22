// Fill out your copyright notice in the Description page of Project Settings.


#include "RSBlockMontageSkipAnimNotifyState.h"
#include "RSDunPlayerCharacter.h"

URSBlockMontageSkipAnimNotifyState::URSBlockMontageSkipAnimNotifyState()
{

}

void URSBlockMontageSkipAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	ARSDunPlayerCharacter* PlayerCharacter = MeshComp->GetOwner<ARSDunPlayerCharacter>();
	if (PlayerCharacter)
	{
		PlayerCharacter->SetIsMontageSkippable(false);
	}
}

void URSBlockMontageSkipAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void URSBlockMontageSkipAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	ARSDunPlayerCharacter* PlayerCharacter = MeshComp->GetOwner<ARSDunPlayerCharacter>();
	if (PlayerCharacter)
	{
		PlayerCharacter->SetIsMontageSkippable(true);
	}
}
