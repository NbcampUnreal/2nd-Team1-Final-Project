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

	// 몬스터 캡슐 컴포넌트에 몬스터 공격을 받지 않도록 무시하는 함수
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_MonsterAttackTrace, ECR_Ignore);

	// 캡슐 컴포넌트의 오버랩 이벤트는 끄고 스켈레탈 메시의 오버랩 이벤트는 켜기
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetGenerateOverlapEvents(true);

	MonsterDataTable = nullptr;

}

void ARSDunMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(detectDelayTimer, this, &ARSDunMonsterCharacter::FindNearPatrolPoint, 0.5f, false);	
	if (UAnimInstance* animInstance = GetMesh()->GetAnimInstance())
	{
		animInstance->OnMontageEnded.AddDynamic(this, &ARSDunMonsterCharacter::OnDeathMontageEnded);
	}

	InitMonsterData();
}

void ARSDunMonsterCharacter::PlayAttackAnim()
{
	// TODO : 퀄리티 업때 PlayBaseAttackAnim()이 비헤이비어 트리에서 몽타주가 실행되고 있으면 호출되지 않도록 최적화 코드 필요.

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (MonsterAttackSkills.Num() > 0)
	{
		// TODO : 지금은 기본 공격 밖에 없어 이렇게 넣었지만 나중에 거리/확률 기반으로 몬스터의 랜덤한 공격이 나가게 설정할 듯
		UAnimMontage* AttackMontage00 = MonsterAttackSkills[0].SkillMontage;

		if (IsValid(AnimInstance) == true && IsValid(AttackMontage00) == true)
		{
			if (AnimInstance->Montage_IsPlaying(AttackMontage00) == false)
			{
				AnimInstance->Montage_Play(AttackMontage00);
				RS_LOG("몬스터가 공격하는 애니메이션이 잘 나왔습니다");
			}
		}
		else
		{
			RS_LOG("해당 몬스터의 스킬 몽타주가 비어있습니다. 데이터 테이블 확인 바람");
		}
	}
	else
	{
		RS_LOG("해당 몬스터의 스킬 구조체가 비어있습니다.");
	}
}

void ARSDunMonsterCharacter::PlayDeathAnim()
{
	PlayAnimMontage(DeathMontage);
	RS_LOG("몬스터가 죽는 애니메이션이 실행되었습니다");
}

void ARSDunMonsterCharacter::PlaySkill_1()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (MonsterAttackSkills.Num() > 0)
	{
		UAnimMontage* AttackMontage01 = MonsterAttackSkills[1].SkillMontage;

		if (IsValid(AnimInstance) == true && IsValid(AttackMontage01) == true)
		{
			if (AnimInstance->Montage_IsPlaying(AttackMontage01) == false)
			{
				AnimInstance->Montage_Play(AttackMontage01);
				RS_LOG("몬스터가 공격하는 애니메이션이 잘 나왔습니다");
			}
		}
	}
	else
	{
		RS_LOG("해당 몬스터의 스킬 구조체가 비어있습니다.");
	}
}

void ARSDunMonsterCharacter::PlaySkill_2()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (MonsterAttackSkills.Num() > 0)
	{
		UAnimMontage* AttackMontage01 = MonsterAttackSkills[1].SkillMontage;

		if (IsValid(AnimInstance) == true && IsValid(AttackMontage01) == true)
		{
			if (AnimInstance->Montage_IsPlaying(AttackMontage01) == false)
			{
				AnimInstance->Montage_Play(AttackMontage01);
				RS_LOG("몬스터가 공격하는 애니메이션이 잘 나왔습니다");
			}
		}
	}
	else
	{
		RS_LOG("해당 몬스터의 스킬 구조체가 비어있습니다.");
	}
}

void ARSDunMonsterCharacter::PlaySkill_3()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (MonsterAttackSkills.Num() > 0)
	{
		UAnimMontage* AttackMontage01 = MonsterAttackSkills[1].SkillMontage;

		if (IsValid(AnimInstance) == true && IsValid(AttackMontage01) == true)
		{
			if (AnimInstance->Montage_IsPlaying(AttackMontage01) == false)
			{
				AnimInstance->Montage_Play(AttackMontage01);
				RS_LOG("몬스터가 공격하는 애니메이션이 잘 나왔습니다");
			}
		}
	}
	else
	{
		RS_LOG("해당 몬스터의 스킬 구조체가 비어있습니다.");
	}
}

/*void ARSDunMonsterCharacter::PlaySkill_4(FVector interrestedPos)
{
	FRotator spawnRot = FRotator::ZeroRotator;
	//GetWorld()->SpawnActor<ARSDunMonsterCharacter>(ARSDunMonsterCharacter::StaticClass(), interrestedPos, spawnRot);
	GetWorld()->SpawnActor<AActor>(servant, interrestedPos, spawnRot);
}*/

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
	RS_LOG("TakeDamage 들어옴");

	if (GetIsDead()) return 0.f;	// 죽으면 데미지 안들어오게 방지

	RS_LOG("TakeDamage 들어와서 데미지까지 실제로 입힘");

	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DecreaseHP(Damage);

	// 데미지 받으면 로그에 추가!
	RS_LOG_F("[%s] 몬스터가 [%s] 에게 %f 데미지를 받았습니다. 체력 : %f / %f",
		*GetName(),
		DamageCauser ? *DamageCauser->GetName() : TEXT("Unknown"),
		Damage,
		GetHP(),
		GetMaxHP()
	);
	
	if (GetHP() <= 0)
	{
		OnDeath();
	}

	return Damage;
}

void ARSDunMonsterCharacter::PerformAttackTrace(int32 SkillIndex)
{
	// 미리 캐싱해둔 몬스터 데이터 중 공격 트레이스 배열 데이터를 가져와서 인덱스에 따라 트레이스가 변경되도록 설정
	const FMonsterAttackTraceData& TraceData = CachedAttackTraceDataArray[SkillIndex];

	FVector Start = GetMesh()->GetSocketLocation(TraceData.SocketLocation);
	Start += GetActorForwardVector() * TraceData.TraceForwardOffset;
	Start += GetActorRightVector() * TraceData.TraceRightOffset;
	Start += GetActorUpVector() * TraceData.TraceUpOffset;

	FVector End = Start + GetActorForwardVector() * TraceData.TraceLength;
	FVector Center = (Start + End) * 0.5f;
	FQuat Rotation = GetActorRotation().Quaternion();
	FVector LocalTraceBoxHalfSize = TraceData.TraceBoxHalfSize;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	// 몬스터 공격이 플레이어에게 적용되게 하려는 Trace입니다. 저희 게임은 플레이어가 1명이니 SweepSingleByChannel을 사용했습니다.
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		Rotation,
		ECC_MonsterAttackTrace,
		FCollisionShape::MakeBox(LocalTraceBoxHalfSize),
		Params
	);

	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("bHit True"));
		AActor* HitActor = HitResult.GetActor();
		if (IsValid(HitActor))
		{
			float AttackDamage = MonsterAttackSkills[SkillIndex].Damage;

			UGameplayStatics::ApplyDamage(HitActor, AttackDamage, GetController(), this, nullptr);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("bHit False"));
	}

	DrawDebugBox(GetWorld(), Center, LocalTraceBoxHalfSize, Rotation, bHit ? FColor::Red : FColor::Green, false, 5.0f);
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

void ARSDunMonsterCharacter::InitMonsterData()
{
	static const FString DataTablePath = TEXT("/Game/Datas/MonsterDataTable.MonsterDataTable");

	UDataTable* LoadedTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *DataTablePath));

	if (LoadedTable)
	{
		MonsterDataTable = LoadedTable;
	}
	else
	{
		RS_LOG("몬스터 데이터테이블 로딩 실패");
	}

	if (MonsterDataTable)
	{
		FMonsterData* Row = MonsterDataTable->FindRow<FMonsterData>(MonsterRowName, TEXT("Set MonsterRowName"));

		if (Row)
		{
			MonsterAttackSkills = Row->MonsterAttackSkills;

			ChangeMaxHP(Row->MaxHP);
			ChangeHP(Row->MaxHP);	// TODO : 이거 뭔가 비효율적인데 InitMaxHP() 함수에 MaxHP랑 HP둘다 조정하는거 만들어주세용 선국님
			ChangeMoveSpeed(Row->MoveSpeed);

			for (const FMonsterAttackSkillData& Skill : MonsterAttackSkills)
			{
				CachedAttackTraceDataArray.Add(Skill.AttackTrace);
			}

			RS_LOG_F("MaxHP가 잘 적용 되었습니다! MoveSpeed : %f", Row->MaxHP);
			RS_LOG_F("MoveSpeed가 잘 적용 되었습니다! MoveSpeed : %f", Row->MoveSpeed);
		}
		else
		{
			RS_LOG_F("MaxHP 적용 실패: %s", *MonsterRowName.ToString());
			RS_LOG_F("MoveSpeed 적용 실패: %s", *MonsterRowName.ToString());
		}
	}
}
