// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunBossSpiderQueenCharacter.h"

ARSDunBossSpiderQueenCharacter::ARSDunBossSpiderQueenCharacter()
{
	TraceBoxHalfSize = FVector(20.f, 20.f, 20.f);
	TraceLength = 150.f;
}

void ARSDunBossSpiderQueenCharacter::PlayBaseAttackAnim()
{
	PlayAnimMontage(BaseAttackMontage);
	UE_LOG(LogTemp, Warning, TEXT("Boss SpiderQueen Attack Success!!"));
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
	FVector Start = GetMesh()->GetSocketLocation("FrontLeg3_R"); // TODO : 여기다 실제 소켓 이름으로 바꿔줘야 함.
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
		ECC_GameTraceChannel1,
		FCollisionShape::MakeBox(TraceBoxHalfSize),
		Params
	);

	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("bHit True"));
		AActor* HitActor = HitResult.GetActor();
		if (IsValid(HitActor))
		{
			UGameplayStatics::ApplyDamage(HitActor, 10.f, GetController(), this, nullptr);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("bHit False"));
	}

	DrawDebugBox(GetWorld(), Center, TraceBoxHalfSize, Rotation, bHit ? FColor::Red : FColor::Green, false, 10.0f);

	UE_LOG(LogTemp, Warning, TEXT("Attack Success DrawDeubugBox!!"));

}
