// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunBossWormCharacter.h"
#include "RogShop/RSMonsterAttackTraceDefine.h"

ARSDunBossWormCharacter::ARSDunBossWormCharacter()
{
}

void ARSDunBossWormCharacter::PlayBaseAttackAnim()
{
	PlayAnimMontage(BaseAttackMontage);
	UE_LOG(LogTemp, Warning, TEXT("BossWorm Attack Success!!"));
}

void ARSDunBossWormCharacter::PlayHitReactAnim()
{
	PlayAnimMontage(HitReactMontage);
	UE_LOG(LogTemp, Warning, TEXT("BossWorm HitReact Success!!"));
}

void ARSDunBossWormCharacter::PlayDeathAnim()
{
	PlayAnimMontage(DeathMontage);
	UE_LOG(LogTemp, Warning, TEXT("BossWorm Death Success!!"));
}

void ARSDunBossWormCharacter::PerformAttackTrace()
{
	FHitResult HitResult;
	FVector Start = GetMesh()->GetSocketLocation(SocketLocation); // TODO : �縷 ���� ���� ������ ����� ���� ���� �̸����� �ٲ���� ��
	Start += GetActorForwardVector() * TraceForwardOffset;
	Start += GetActorRightVector() * TraceRightOffset;
	Start += GetActorUpVector() * TraceUpOffset;

	FVector End = Start + GetActorForwardVector() * TraceLength;
	FQuat Rotation = GetActorRotation().Quaternion();

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	// DrawDebugBox���� ���� ����
	FVector Center = (Start + End) * 0.5f;

	// ���� ������ �÷��̾�� ����ǰ� �Ϸ��� Trace�Դϴ�. ���� ������ �÷��̾ 1���̴� SweepSingleByChannel�� ����߽��ϴ�.
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
