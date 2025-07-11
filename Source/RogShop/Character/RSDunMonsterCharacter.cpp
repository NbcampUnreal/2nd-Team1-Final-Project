// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunMonsterCharacter.h"
#include "RSDunPlayerCharacter.h"
#include "RogShop/RSMonsterAttackTraceDefine.h"
#include "RogShop/UtilDefine.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ProgressBar.h"
#include "RSDataSubsystem.h"
#include "Perception/AISense_Damage.h"
#include "RSMonsterHPBarWidget.h"

TArray<ARSDunMonsterCharacter*> ARSDunMonsterCharacter::AllMonsters;

ARSDunMonsterCharacter::ARSDunMonsterCharacter()
{
	// 몬스터 스폰 테스트할 때 AIController가 스폰안되는 문제가 있어서 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//Navigation, NavLink
	navGenerationRadius = 1000.0f;
	navRemovalRadius = 1500.0f;
	jumpForce = 100.0f;

	navInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavInvoker"));	
	navInvoker->SetGenerationRadii(navGenerationRadius, navRemovalRadius);

	// 몬스터 체력바랑 데미지 텍스트 위젯 생성 및 설정(BP랑 연결은 에디터의 위젯 클래스에서 설정)
	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	OverheadWidget->SetupAttachment(GetMesh());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::World);
	OverheadWidget->SetDrawSize(FVector2D(1920.f, 500.f));
	OverheadWidget->SetRelativeLocation(FVector(0.f, 0.f, 250.f)); // 필요 시 위치 조정
	OverheadWidget->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	DamageTextWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageText"));
	DamageTextWidget->SetupAttachment(GetMesh());
	DamageTextWidget->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	DamageTextWidget->SetWidgetSpace(EWidgetSpace::Screen);

	//Patrol
	maxDetectPatrolRoute = 2000.f;

	// 몬스터 캡슐 컴포넌트에 몬스터 공격을 받지 않도록 무시하는 함수
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_MonsterAttackTrace, ECR_Ignore);

	// 캡슐컴포넌트와 스켈레탈 메시의 오버렙 이벤트를 끈다. << 플레이어 공격이 트레이스 방식으로 변경되서 오버렙 이벤트가 필요X
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetGenerateOverlapEvents(false);

	DrawDebugLineSeconds = 5.0f;
	DrawDebugLineThickness = 5.0f;

	meleeAtkRange = 150.0f;
	strafeRange = 500.0f;
	bIsPlayingAnim = false;
	bIsInterruptable = true;
	interruptResistance = 0.f;
}

void ARSDunMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(detectDelayTimer, this, &ARSDunMonsterCharacter::FindNearPatrolPoint, 0.5f, false);	
	GetWorld()->GetTimerManager().SetTimer(MonsterWidgetComponentRotationTimer, this, &ARSDunMonsterCharacter::UpdateEnemyWidgetComponentRotation, 0.1f, true);

	if (UAnimInstance* animInstance = GetMesh()->GetAnimInstance())
	{
		animInstance->OnMontageEnded.AddDynamic(this, &ARSDunMonsterCharacter::OnEveryMontageEnded);
	}

	AllMonsters.Add(this);	// 새로운 몬스터는 몬스터 배열에 추가 < 이유는 모든 몬스터의 HP바 회전을 위함.

}

void ARSDunMonsterCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	AllMonsters.Remove(this);
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

void ARSDunMonsterCharacter::PlaySpawnAnim()
{
	if (SpawnMontage)
	{
		PlayAnimMontage(SpawnMontage);
		bIsPlayingAnim = true;
		bIsInterruptable = false;
	}
}

void ARSDunMonsterCharacter::PlayAction(int32 actionIdx, FVector interestedPos)
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();

	if (MonsterAttackSkills.Num() > actionIdx)
	{
		UAnimMontage* action = MonsterAttackSkills[actionIdx].SkillMontage;
		if (animInstance && action)
		{
			if (!bIsPlayingAnim)
			{
				animInstance->Montage_Play(action);
				bIsPlayingAnim = true;
				bIsInterruptable = false;
				skillActionIdx = actionIdx;
			}
		}
	}
	const FMonsterAttackSkillData& actionData = MonsterAttackSkills[actionIdx];
	
	if (actionData.skillType == ESkillType::Utillity) // 공격이 아닌 경우
	{
		UtillitySkill(actionIdx, interestedPos);
	}
}

void ARSDunMonsterCharacter::UtillitySkill_Implementation(int32 actionIdx, FVector interestedPos)
{

}

void ARSDunMonsterCharacter::OnEveryMontageEnded(UAnimMontage* montage, bool bInterrupted)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (!AnimInstance->Montage_IsPlaying(montage))
	{
		if (montage == DeathMontage)//사망 하는 경우
		{
			Destroy();
		}
		bIsPlayingAnim = false;
		bIsInterruptable = true;
	}
}

float ARSDunMonsterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UAISense_Damage* damagePerception;
	RS_LOG("TakeDamage 들어옴");

	if (GetIsDead()) return 0.f;	// 죽으면 데미지 안들어오게 방지

	RS_LOG("TakeDamage 들어와서 데미지까지 실제로 입힘");

	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DecreaseHP(Damage);
	UpdateHPUI();

	// 데미지 받으면 로그에 추가!
	RS_LOG_F("[%s] 몬스터가 [%s] 에게 %f 데미지를 받았습니다. 체력 : %f / %f",
		*GetName(),
		DamageCauser ? *DamageCauser->GetName() : TEXT("Unknown"),
		Damage,
		GetHP(),
		GetMaxHP()
	);

	//피해를 받은 경우 공격 대상을 찾는다
	damagePerception->ReportDamageEvent(GetWorld(), this, DamageCauser, Damage, this->GetActorLocation(), DamageCauser->GetActorLocation(), TEXT("Default"));


	return Damage;
}

void ARSDunMonsterCharacter::PerformAttackTrace()
{
	const FMonsterAttackSkillData& actionData = MonsterAttackSkills[skillActionIdx];
	if (actionData.skillType == ESkillType::Melee) // 근접 공격인 경우
	{
		// 미리 캐싱해둔 몬스터 데이터 중 공격 트레이스 배열 데이터를 가져와서 인덱스에 따라 트레이스가 변경되도록 설정	
		const FMonsterAttackTraceData& TraceData = CachedAttackTraceDataArray[skillActionIdx];
		FVector Start = GetMesh()->GetSocketLocation(TraceData.SocketLocation);
		Start += GetActorForwardVector() * TraceData.TraceForwardOffset;
		Start += GetActorRightVector() * TraceData.TraceRightOffset;
		Start += GetActorUpVector() * TraceData.TraceUpOffset;

		FVector End = Start + GetActorForwardVector() * TraceData.TraceLength;
		//FVector End = Start + GetActorForwardVector() * 500.0f;
		FVector Center = (Start + End) * 0.5f;
		FQuat Rotation = GetActorRotation().Quaternion();

		FVector LocalTraceBoxHalfSize = TraceData.TraceBoxHalfSize;
		FVector StartTraceBoxHalfSize = LocalTraceBoxHalfSize * 0.9f;
		LocalTraceBoxHalfSize.X += TraceData.TraceLength * 0.5f;

		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

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
			RS_LOG("bHit True! 무언가 공격에 맞았습니다!");
			AActor* HitActor = HitResult.GetActor();
			bool bPlayerHit = IsValid(HitActor) && HitActor->IsA(ARSDunPlayerCharacter::StaticClass());
			// bool bPlayerHit = IsValid(HitActor); // TODO: MonsterPawn Block->Ignore 설정 가능하면 이 코드로 변경

			if (bPlayerHit)
			{
				float AttackDamage = MonsterAttackSkills[skillActionIdx].Damage;

				UGameplayStatics::ApplyDamage(HitActor, AttackDamage, GetController(), this, nullptr);

				RS_LOG_F("[%s] 몬스터가 [%s] 에게 %f 데미지를 주었습니다!",
					*GetName(),
					HitActor ? *HitActor->GetName() : TEXT("Unknown"),
					AttackDamage
				);
			}
			else
			{
				RS_LOG("플레이어가 아닌 Pawn(몬스터, 가짜 Pawn 등)이 공격에 맞았습니다!");
			}

			RS_DRAW_DEBUG_BOX(GetWorld(), Start, StartTraceBoxHalfSize, Rotation, bPlayerHit ? FColor::Red : FColor::Green, false, DrawDebugLineSeconds, 0, 1.0f);
			RS_DRAW_DEBUG_LINE(GetWorld(), Start, End, bPlayerHit ? FColor::Red : FColor::Yellow, false, DrawDebugLineSeconds, 2, DrawDebugLineThickness);
			RS_DRAW_DEBUG_BOX(GetWorld(), Center, LocalTraceBoxHalfSize, Rotation, bPlayerHit ? FColor::Red : FColor::Green, false, DrawDebugLineSeconds, 0, 1.0f);

		}
		else
		{
			RS_DRAW_DEBUG_BOX(GetWorld(), Start, StartTraceBoxHalfSize, Rotation, FColor::Green, false, DrawDebugLineSeconds, 0, 1.0f);
			RS_DRAW_DEBUG_LINE(GetWorld(), Start, End, FColor::Yellow, false, DrawDebugLineSeconds, 2, DrawDebugLineThickness);
			RS_DRAW_DEBUG_BOX(GetWorld(), Center, LocalTraceBoxHalfSize, Rotation, FColor::Green, false, DrawDebugLineSeconds, 0, 1.0f);
			RS_LOG("몬스터의 공격에 Pawn이 아닌 물체가 맞았거나 아무도 맞지 않았습니다!");
		}
	}
	else if (actionData.skillType == ESkillType::Range) // 원거리 공격인 경우
	{
		if (projectile == nullptr)
		{
			return;
		}
		// TODO: 원거리 공격 로직 작성
		const FMonsterAttackTraceData& TraceData = CachedAttackTraceDataArray[skillActionIdx];
		FVector Start = GetMesh()->GetSocketLocation(TraceData.SocketLocation);
		FRotator socketRot = GetMesh()->GetSocketRotation(TraceData.SocketLocation);
		Start += GetActorForwardVector() * TraceData.TraceForwardOffset;
		Start += GetActorRightVector() * TraceData.TraceRightOffset;
		Start += GetActorUpVector() * TraceData.TraceUpOffset;

		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Instigator = GetInstigator();
		UWorld* world = GetWorld();
		AMonsterProjectileBase* rangedBullet = world->SpawnActor<AMonsterProjectileBase>(projectile, Start, socketRot, spawnParams);
		if (rangedBullet)
		{
			const FVector launchDir = socketRot.Vector();
			rangedBullet->Fire(launchDir);
		}
	}	
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

//	RS_DRAW_DEBUG_SPHERE(GetWorld(), GetActorLocation(), maxDetectPatrolRoute, 16, FColor::Red, false, 60.f, 0, 1.0f);

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
	Super::OnDeath();

	AController* ctrl = GetController();
	if (ctrl)
	{
		ctrl->UnPossess();
		ctrl->Destroy();
		bIsInterruptable = false;
	}
}

void ARSDunMonsterCharacter::InitMonsterData()
{
	if (GetFMonsterData())
	{
		MonsterAttackSkills = GetFMonsterData()->MonsterAttackSkills;

		ChangeMaxHP(GetFMonsterData()->MaxHP);
		ChangeHP(GetFMonsterData()->MaxHP);	// TODO : 이거 뭔가 비효율적인데 InitMaxHP() 함수에 MaxHP랑 HP둘다 조정하는거 만들어주세용 선국님
		ChangeMoveSpeed(GetFMonsterData()->MoveSpeed);

		for (const FMonsterAttackSkillData& Skill : MonsterAttackSkills)
		{
			CachedAttackTraceDataArray.Add(Skill.AttackTrace);
		}

		RS_LOG_F("MaxHP가 잘 적용 되었습니다! MaxHP : %f", GetFMonsterData()->MaxHP);
		RS_LOG_F("MoveSpeed가 잘 적용 되었습니다! MoveSpeed : %f", GetFMonsterData()->MoveSpeed);
	}
	else
	{
		RS_LOG_F("MaxHP 적용 실패: %s", *MonsterRowName.ToString());
		RS_LOG_F("MoveSpeed 적용 실패: %s", *MonsterRowName.ToString());
	}
}

int32 ARSDunMonsterCharacter::GetActionLength()
{
	return MonsterAttackSkills.Num();
}

bool ARSDunMonsterCharacter::GetIsMeleeSkill(int32 actionIdx)
{
	return MonsterAttackSkills[actionIdx].skillType == ESkillType::Melee;
}

bool ARSDunMonsterCharacter::GetIsPlayingAnim()
{
	return bIsPlayingAnim;
}

float ARSDunMonsterCharacter::GetAtkRange()
{
	return meleeAtkRange;
}

float ARSDunMonsterCharacter::GetStrafeRange()
{
	return strafeRange;
}

const TArray<ARSDunMonsterCharacter*>& ARSDunMonsterCharacter::GetAllMonsters()
{
	return AllMonsters;
}

const FMonsterData* ARSDunMonsterCharacter::GetFMonsterData()
{
	UGameInstance* CurGameInstance = GetGameInstance();
	if (!CurGameInstance)
	{
		return nullptr;
	}

	URSDataSubsystem* DataSubsystem = CurGameInstance->GetSubsystem<URSDataSubsystem>();
	if (!DataSubsystem)
	{
		return nullptr;
	}

	UDataTable* MonsterDataTable = DataSubsystem->Monster;
	if (!MonsterDataTable)
	{
		RS_LOG("몬스터 데이터테이블 로딩 실패");
		return nullptr;
	}

	FMonsterData* MonsterDataRow = MonsterDataTable->FindRow<FMonsterData>(MonsterRowName, TEXT("Get MonsterDataRow"));
	return MonsterDataRow;
}

void ARSDunMonsterCharacter::UpdateHPUI()
{
	UpdateOverheadEnemyHP();
}

// 적이 가진 위젯 컴포넌트가 항상 카메라를 바라보게 회전시키는 함수
void ARSDunMonsterCharacter::UpdateEnemyWidgetComponentRotation()
{
	if (!OverheadWidget || !DamageTextWidget || !GetWorld()) return;

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return;

	FVector CameraLocation;
	FRotator CameraRotation;
	PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FRotator NewRotation = FRotator(0, CameraRotation.Yaw + 180.0f, 0);

	// 기존 회전값과 비교해서 변할 때만 업데이트 (불필요한 연산 방지)
	if (!NewRotation.Equals(OverheadWidget->GetComponentRotation(), 0.1f))
	{
		OverheadWidget->SetWorldRotation(NewRotation);
		DamageTextWidget->SetWorldRotation(NewRotation);
	}
}

void ARSDunMonsterCharacter::UpdateOverheadEnemyHP()
{
	if (!OverheadWidget)
	{
		return;
	}

	// 1. 캐릭터 머리 위에 연결할 위젯의 실체 가져오기
	UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstance)
	{
		return;
	}

	// 2. 타입 캐스팅: UUserWidget → URSMonsterHPBarWidget
	URSMonsterHPBarWidget* HPWidget = Cast<URSMonsterHPBarWidget>(OverheadWidgetInstance);
	if (!HPWidget)
	{
		return;
	}
	
	// HPPercent = 0.0 ~ 1.0 범위의 값이 나오도록 설정
	const float HPPercent = (GetMaxHP() > 0.f) ? (float)GetHP() / (float)GetMaxHP() : 0.f;
	HPWidget->UpdateTargetPercent(HPPercent);

}

FName ARSDunMonsterCharacter::GetMonsterRowName() const
{
	return MonsterRowName;
}
