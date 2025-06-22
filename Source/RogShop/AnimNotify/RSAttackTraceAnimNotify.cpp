// Fill out your copyright notice in the Description page of Project Settings.


#include "RSAttackTraceAnimNotify.h"
#include "RSDunMonsterCharacter.h"
#include "RogShop/UtilDefine.h"

void URSAttackTraceAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	RS_LOG_DEBUG("Notify Success!!");
	AActor* Owner = MeshComp->GetOwner();

	if (MeshComp && Owner)
	{
		RS_LOG_DEBUG("Notify GetOwner!!");
		ARSDunMonsterCharacter* DunMonsterCharacter = Cast<ARSDunMonsterCharacter>(Owner);

		if (DunMonsterCharacter)
		{
			RS_LOG_DEBUG("Notify DunMonsterCharacter Nice Casting!!");
			// 몬스터가 갖고 있는 스킬들이 TArray로 저장되어 각 공격 스킬을 인덱스에 따라 호출 가능
			DunMonsterCharacter->PerformAttackTrace();  // TODO : 나중에 랜덤한 0~4 에서 나오는 변수를 집어 넣을 것.
		}
	}
}
