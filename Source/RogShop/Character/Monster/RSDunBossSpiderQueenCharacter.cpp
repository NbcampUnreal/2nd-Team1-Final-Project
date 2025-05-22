// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunBossSpiderQueenCharacter.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/RSMonsterAttackTraceDefine.h"

ARSDunBossSpiderQueenCharacter::ARSDunBossSpiderQueenCharacter()
{
}

void ARSDunBossSpiderQueenCharacter::PlayBaseAttackAnim()
{
	// TODO : 퀄리티 업때 PlayBaseAttackAnim()이 비헤이비어 트리에서 몽타주가 실행되고 있으면 호출되지 않도록 최적화 코드 필요.

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance) == true && IsValid(BaseAttackMontage) == true)
	{
		if (AnimInstance->Montage_IsPlaying(BaseAttackMontage) == false)
		{
			AnimInstance->Montage_Play(BaseAttackMontage);
			RS_LOG("거미 여왕 공격합니다.");
		}
	}
}

void ARSDunBossSpiderQueenCharacter::PlayHitReactAnim()
{
	PlayAnimMontage(HitReactMontage);
	UE_LOG(LogTemp, Warning, TEXT("Boss SpiderQueen HitReact Success!!"));
}

void ARSDunBossSpiderQueenCharacter::PlayDeathAnim()
{
	PlayAnimMontage(DeathMontage);
	UE_LOG(LogTemp, Warning, TEXT("Boss SpiderQueen Death Success!!"));
}

void ARSDunBossSpiderQueenCharacter::PerformAttackTrace()
{
	FHitResult HitResult;
	FVector Start = GetMesh()->GetSocketLocation("FrontLeg3_R"); // 여기다 실제 소켓 이름으로 바꿔줘야 함.
	Start += GetActorForwardVector() * TraceForwardOffset;
	Start += GetActorRightVector() * TraceRightOffset;
	Start += GetActorUpVector() * TraceUpOffset;

	FVector End = Start + GetActorForwardVector() * TraceLength;
	FQuat Rotation = GetActorRotation().Quaternion();

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	// DrawDebugBox에서 쓰는 변수
	FVector Center = (Start + End) * 0.5f;

	// 몬스터 공격이 플레이어에게 적용되게 하려는 Trace입니다. 저희 게임은 플레이어가 1명이니 SweepSingleByChannel을 사용했습니다.
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		Rotation,
		ECC_MonsterAttackTrace,
		FCollisionShape::MakeBox(TraceBoxHalfSize),
		Params
	);

	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("bHit True"));
		AActor* HitActor = HitResult.GetActor();
		if (IsValid(HitActor))
		{
			UGameplayStatics::ApplyDamage(HitActor, 1.f, GetController(), this, nullptr);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("bHit False"));
	}

	DrawDebugBox(GetWorld(), Center, TraceBoxHalfSize, Rotation, bHit ? FColor::Red : FColor::Green, false, 10.0f);

	UE_LOG(LogTemp, Warning, TEXT("Attack Success DrawDeubugBox!!"));

}
