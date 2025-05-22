// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunEliteIceGolemCharacter.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/RSMonsterAttackTraceDefine.h"

void ARSDunEliteIceGolemCharacter::PlayBaseAttackAnim()
{
	// TODO : ����Ƽ ���� PlayBaseAttackAnim()�� �����̺�� Ʈ������ ��Ÿ�ְ� ����ǰ� ������ ȣ����� �ʵ��� ����ȭ �ڵ� �ʿ�.

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance) == true && IsValid(BaseAttackMontage) == true)
	{
		if (AnimInstance->Montage_IsPlaying(BaseAttackMontage) == false)
		{
			AnimInstance->Montage_Play(BaseAttackMontage);
			RS_LOG("�����մϴ�.");
		}
	}
}

void ARSDunEliteIceGolemCharacter::PlayHitReactAnim()
{
	PlayAnimMontage(HitReactMontage);
	UE_LOG(LogTemp, Warning, TEXT("Boss SpiderQueen HitReact Success!!"));
}

void ARSDunEliteIceGolemCharacter::PlayDeathAnim()
{
	PlayAnimMontage(DeathMontage);
	UE_LOG(LogTemp, Warning, TEXT("Boss SpiderQueen Death Success!!"));
}

void ARSDunEliteIceGolemCharacter::PerformAttackTrace()
{
	FHitResult HitResult;
	FVector Start = GetMesh()->GetSocketLocation("FrontLeg3_R"); // ����� ���� ���� �̸����� �ٲ���� ��.
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
