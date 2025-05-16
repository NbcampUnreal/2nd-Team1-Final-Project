// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunMonsterCharacter.h"
#include "MeleeAttackBoxComponent.h"

ARSDunMonsterCharacter::ARSDunMonsterCharacter()
{
	MeleeAttackBoxComponent = CreateDefaultSubobject<UMeleeAttackBoxComponent>(TEXT("MeleeAttackBoxComponent"));
	MeleeAttackBoxComponent->SetupAttachment(RootComponent);

	//Navigation, NavLink
	navGenerationRadius = 500.0f;
	navRemovalRadius = 750.0f;
	jumpForce = 0.0f;

	navInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavInvoker"));	
	navInvoker->SetGenerationRadii(navGenerationRadius, navRemovalRadius);

	//Patrol
	maxDetectPatrolRoute = 2000.f;
	detectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectPatrolRoute"));
	detectSphere->SetupAttachment(RootComponent);
	detectSphere->SetSphereRadius(maxDetectPatrolRoute);
	detectSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	detectSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	detectSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	detectSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	detectSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	detectSphere->SetGenerateOverlapEvents(true);
}

void ARSDunMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();
	FindNearPatrolPoint();
	
}

void ARSDunMonsterCharacter::PlayBaseAttackAnim()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack Complete!!"));
}

void ARSDunMonsterCharacter::PlayHitReactAnim()
{
	UE_LOG(LogTemp, Warning, TEXT("HitReact Complete!!"));
}

void ARSDunMonsterCharacter::PlayDeathAnim()
{
	UE_LOG(LogTemp, Warning, TEXT("Death Complete!!"));
}

float ARSDunMonsterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	/*if (bIsDead) return 0.0f;*/ // TODO : 만약 선국님이 bIsDead 변수 만들어 주면 넣을 로직

	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	HP -= damage;

	// 데미지 받으면 로그에 추가! (임시)
	UE_LOG(LogTemp, Warning, TEXT("[%s] took %.1f damage from [%s]"),
		*GetName(),
		damage,
		DamageCauser ? *DamageCauser->GetName() : TEXT("Unknown"));

	
	if (HP <= 0)
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
/*	TArray<AActor*> overlappingActors;
	detectSphere->GetOverlappingActors(overlappingActors, ATargetPoint::StaticClass());
	
	for (AActor* act : overlappingActors)
	{
		ATargetPoint* target = Cast<ATargetPoint>(act);
		if (target)
		{
			patrolPoints.Add(target);
			UE_LOG(LogTemp, Error, TEXT("iswork?"));
		}
	}*/
	TArray<FOverlapResult> overlapResults;
	FCollisionQueryParams collisionQueryParms(NAME_None, false, this);
	bool bResult = GetWorld()->OverlapMultiByChannel(
		overlapResults,
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(300.f),
		collisionQueryParms
	);

	if (bResult)
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
	PlayDeathAnim();
}
