// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunMonsterCharacter.h"
#include "RogShop/RSMonsterAttackTraceDefine.h"
#include "RogShop/UtilDefine.h"
#include "Components/CapsuleComponent.h"

ARSDunMonsterCharacter::ARSDunMonsterCharacter()
{
	// 몬스터 스폰 테스트할 때 AIController가 스폰안되는 문제가 있어서 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ARSDunMonsterCharacter::StaticClass();

	//Navigation, NavLink
	navGenerationRadius = 1000.0f;
	navRemovalRadius = 1500.0f;
	jumpForce = 100.0f;

	navInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavInvoker"));	
	navInvoker->SetGenerationRadii(navGenerationRadius, navRemovalRadius);

	//Patrol
	maxDetectPatrolRoute = 2000.f;

	// 몬스터 공격 트레이스 관련 변수 초기화
	TraceBoxHalfSize = FVector(60.f, 60.f, 80.f);
	TraceLength = 0.f;
	TraceForwardOffset = 0.f;
	TraceRightOffset = 0.f;
	TraceUpOffset = 0.f;

	// 몬스터 캡슐 컴포넌트에 몬스터가 공격을 받아서 그것을 무시하도록 한 코드
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_MonsterAttackTrace, ECR_Ignore);

}

void ARSDunMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(detectDelayTimer, this, &ARSDunMonsterCharacter::FindNearPatrolPoint, 0.5f, false);	
	if (UAnimInstance* animInstance = GetMesh()->GetAnimInstance())
	{
		animInstance->OnMontageEnded.AddDynamic(this, &ARSDunMonsterCharacter::OnDeathMontageEnded);
	}
}

void ARSDunMonsterCharacter::PlayBaseAttackAnim()
{
	// TODO : 퀄리티 업때 PlayBaseAttackAnim()이 비헤이비어 트리에서 몽타주가 실행되고 있으면 호출되지 않도록 최적화 코드 필요.

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance) == true && IsValid(BaseAttackMontage) == true)
	{
		if (AnimInstance->Montage_IsPlaying(BaseAttackMontage) == false)
		{
			AnimInstance->Montage_Play(BaseAttackMontage);
			RS_LOG("몬스터가 공격하는 애니메이션이 잘 나왔습니다.");
		}
	}
}

void ARSDunMonsterCharacter::PlayDeathAnim()
{
	PlayAnimMontage(DeathMontage);
	RS_LOG("몬스터가 죽었습니다.");
}

void ARSDunMonsterCharacter::PlaySkill_1()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance) == true && IsValid(SkillMontage_1) == true)
	{
		if (AnimInstance->Montage_IsPlaying(SkillMontage_1) == false)
		{
			AnimInstance->Montage_Play(SkillMontage_1);
		}
	}
}

void ARSDunMonsterCharacter::PlaySkill_2()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance) == true && IsValid(SkillMontage_2) == true)
	{
		if (AnimInstance->Montage_IsPlaying(SkillMontage_2) == false)
		{
			AnimInstance->Montage_Play(SkillMontage_2);
		}
	}
}

void ARSDunMonsterCharacter::PlaySkill_3()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance) == true && IsValid(SkillMontage_3) == true)
	{
		if (AnimInstance->Montage_IsPlaying(SkillMontage_3) == false)
		{
			AnimInstance->Montage_Play(SkillMontage_3);
		}
	}
}

void ARSDunMonsterCharacter::OnDeathMontageEnded(UAnimMontage* montage, bool bInterrupted)
{
	if (montage == DeathMontage)
	{
		//GetWorld()->GetTimerManager().SetTimer(animPlayDelayTimer, this, &ARSDunMonsterCharacter::AIDestroy, 5.0f, false);
		Destroy();
	}
}

float ARSDunMonsterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// TODO : bIsDead 변수 나중에 넣을 로직
	/*if (bIsDead) return 0.0f;*/ 

	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	DecreaseHP(damage);

	// 데미지 받으면 로그에 추가! (임시)
	UE_LOG(LogTemp, Warning, TEXT("[%s] took %.1f damage from [%s]"),
		*GetName(),
		damage,
		DamageCauser ? *DamageCauser->GetName() : TEXT("Unknown"));

	
	if (GetHP() <= 0)
	{
		OnDeath();
	}

	// TODO : 만약 영빈님이 블랙보드에 IsHit 같은 변수 있다고 하면 넣을 로직
	/*if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BBComponent = AIController->GetBlackboardComponent())
		{
			BBComponent->SetValueAsBool(TEXT("IsHit"), true);
		}
	}*/

	return damage;
}

void ARSDunMonsterCharacter::PerformAttackTrace()
{
	FHitResult HitResult;
	FVector Start = GetMesh()->GetSocketLocation(SocketLocation);
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

	DrawDebugBox(GetWorld(), Center, TraceBoxHalfSize, Rotation, bHit ? FColor::Red : FColor::Green, false, 5.0f);

	RS_LOG("몬스터 공격이 성공해서 공격 트레이스 디버그 박스를 그립니다.");

}

void ARSDunMonsterCharacter::JumpTo(FVector destination)
{
	FVector sp = GetActorLocation();
	FVector launchVelocity;
	destination.Z += jumpForce;
	bool bSuggest = UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this,
		launchVelocity,
		sp,
		destination,
		0.0f,
		0.5f
	);

	if (bSuggest)
	{
		LaunchCharacter(launchVelocity, true, true);
	}
}

void ARSDunMonsterCharacter::FindNearPatrolPoint()
{
	TArray<FOverlapResult> overlapResults;
	FCollisionQueryParams collisionQueryParms(NAME_None, false, this);
	bool bResult = GetWorld()->OverlapMultiByChannel(
		overlapResults,
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(maxDetectPatrolRoute),
		collisionQueryParms
	);
//	DrawDebugSphere(GetWorld(), GetActorLocation(), maxDetectPatrolRoute, 16, FColor::Red, false, 60.f);

	if (!overlapResults.IsEmpty())
	{
		for (auto const& overlapResult : overlapResults)
		{
			ATargetPoint* target = Cast<ATargetPoint>(overlapResult.GetActor());
			if (IsValid(target))
			{
				patrolPoints.Add(target);
			}
		}
	}
}

TArray<AActor*> ARSDunMonsterCharacter::GetPatrolPoint()
{
	return patrolPoints;
}

void ARSDunMonsterCharacter::OnDeath()
{
	AController* ctrl = GetController();
	if (ctrl)
	{
		ctrl->UnPossess();
		ctrl->Destroy();
	}

	PlayDeathAnim();
}
