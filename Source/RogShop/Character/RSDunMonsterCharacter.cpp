// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunMonsterCharacter.h"
#include "MeleeAttackBoxComponent.h"

ARSDunMonsterCharacter::ARSDunMonsterCharacter()
{
	MeleeAttackBoxComponent = CreateDefaultSubobject<UMeleeAttackBoxComponent>(TEXT("MeleeAttackBoxComponent"));
	MeleeAttackBoxComponent->SetupAttachment(RootComponent);

	//Navigation, NavLink
	navGenerationRadius = 1000.0f;
	navRemovalRadius = 1500.0f;
	jumpForce = 100.0f;

	navInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavInvoker"));	
	navInvoker->SetGenerationRadii(navGenerationRadius, navRemovalRadius);

	//Patrol
	maxDetectPatrolRoute = 2000.f;
}

void ARSDunMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(detectDelayTimer, this, &ARSDunMonsterCharacter::FindNearPatrolPoint, 0.5f, false);	
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
		ECollisionChannel::ECC_GameTraceChannel2,
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