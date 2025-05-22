// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunBossFlowerCharacter.h"
#include "RogShop/RSMonsterAttackTraceDefine.h"

ARSDunBossFlowerCharacter::ARSDunBossFlowerCharacter()
{
}

void ARSDunBossFlowerCharacter::PlayBaseAttackAnim()
{
	PlayAnimMontage(BaseAttackMontage);
	UE_LOG(LogTemp, Warning, TEXT("BossFlower Attack Success!!"));
}

void ARSDunBossFlowerCharacter::PlayHitReactAnim()
{
	PlayAnimMontage(HitReactMontage);
	UE_LOG(LogTemp, Warning, TEXT("BossFlower HitReact Success!!"));
}

void ARSDunBossFlowerCharacter::PlayDeathAnim()
{
	PlayAnimMontage(DeathMontage);
	UE_LOG(LogTemp, Warning, TEXT("BossFlower Death Success!!"));
}

void ARSDunBossFlowerCharacter::PerformAttackTrace()
{
	FHitResult HitResult;
	FVector Start = GetMesh()->GetSocketLocation("Head"); // ����� ���� ���� �̸����� �ٲ���� ��.
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