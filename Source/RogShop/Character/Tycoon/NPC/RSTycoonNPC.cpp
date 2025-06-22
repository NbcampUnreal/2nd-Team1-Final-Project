// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonNPC.h"

#include "AIController.h"
#include "RSTycoonGameModeBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "RogShop/UtilDefine.h"

ARSTycoonNPC::ARSTycoonNPC()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	InteractSphere = CreateDefaultSubobject<USphereComponent>("InteractSphere");
	InteractSphere->SetupAttachment(RootComponent);

	InteractSphere->SetSphereRadius(60.f);
	InteractSphere->SetCanEverAffectNavigation(false);
	InteractSphere->SetGenerateOverlapEvents(true);
	InteractSphere->SetCollisionProfileName(TEXT("OverlapAll"));

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->bUseRVOAvoidance = true;
	Movement->AvoidanceConsiderationRadius = 100.f;
}


void ARSTycoonNPC::MoveToTarget(FVector Location, AActor* Target)
{
	// 타겟 액터를 향해 이동
	EPathFollowingRequestResult::Type Result = Cast<AAIController>(GetController())->MoveToLocation
	(
		Location, //타겟
		-1,
		true,
		true,
		false
	);

	RS_DRAW_DEBUG_SPHERE(GetWorld(), Location, 50, 30, FColor::Red, false, 5, 0, 1.0f);

	if (Result == EPathFollowingRequestResult::Type::Failed)
	{
		RS_LOG_C("NPC가 경로를 찾지 못해 이동 실패", FColor::Red);

		//재시도
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [&, Location, Target]()
		{
			MoveToTarget(Location, Target);
		}, 0.1f, false);
	}
	else
	{
		MoveTarget = Target;
		InteractSphere->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);

		//네비게이션이 빌드되는 순간 혹은 네비게이션이 Invoker를 인식해 빌드되기 전에 명령이 들어오면 멈춰버리는 버그가 있음
		//해당 버그를 해결하기위해 성공해도 확인을 다시 해주게 제작
		//될 때 까지 재시도
		if (!GetWorldTimerManager().TimerExists(MoveTargetTimer) || !GetWorldTimerManager().IsTimerActive(MoveTargetTimer))
		{
			GetWorldTimerManager().SetTimer(MoveTargetTimer, [&, Location, Target]()
			{
				if (MoveTarget == nullptr)
				{
					GetWorldTimerManager().ClearTimer(MoveTargetTimer);
					return;
				}

				if (MoveTarget != nullptr && GetCharacterMovement()->Velocity.SquaredLength() < 0.1f * 0.1f)
				{
					RS_LOG_C("NPC가 네비게이션 문제로 움직이지 않음", FColor::Red);

					GetController<AAIController>()->MoveToLocation
					(
						GetActorLocation() + FVector(FMath::RandRange(0, 50), FMath::RandRange(0, 50), 0),
						-1, true, false, false
					);

					FTimerHandle RetryTimer;
					GetWorldTimerManager().SetTimer(RetryTimer, [&]()
					{
						MoveToTarget(Location, Target);
					}, 0.2f, false);
				}
			}, 0.3f, true);
		}
	}
}

void ARSTycoonNPC::StopAllAction()
{
	GetWorldTimerManager().ClearAllTimersForObject(this);
	
	MoveTarget = nullptr;
}

void ARSTycoonNPC::InteractTarget(AActor* TargetActor)
{
}

void ARSTycoonNPC::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>()->AddNPC(this);
	InteractSphere->OnComponentBeginOverlap.AddDynamic(this, &ARSTycoonNPC::OnInteract);
}

void ARSTycoonNPC::Destroyed()
{
	if (GetWorld()->IsGameWorld())
	{
		GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>()->RemoveNPC(this);
	}

	Super::Destroyed();
}

void ARSTycoonNPC::OnInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                              const FHitResult& SweepResult)
{
	if (MoveTarget == nullptr || OtherActor != MoveTarget)
	{
		return;
	}

	//상호 작용함
	MoveTarget = nullptr;
	InteractSphere->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	Cast<AAIController>(GetController())->StopMovement();

	InteractTarget(OtherActor);
	OnInteractTarget.Broadcast(OtherActor);
}
