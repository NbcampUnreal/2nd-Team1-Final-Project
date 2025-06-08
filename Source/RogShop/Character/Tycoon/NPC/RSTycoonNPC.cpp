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
		Location //타겟
		// 30.f	// 도착 판정 반경
		// true,	// 충돌 영역이 겹치면 도착으로 간주
		// true,	// 경로 탐색 사용
		// false	// 목적지를 네비게이션 메시에 투영(Projection)하지 않음
	);

	RS_DRAW_DEBUG_SPHERE(GetWorld(), Location, 50, 30, FColor::Red, false, 5, 0, 1.0f);
	
	if (Result == EPathFollowingRequestResult::Type::Failed)
	{
		RS_LOG_C("NPC 이동 실패", FColor::Red);

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
	}
}

void ARSTycoonNPC::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>()->AddNPC(this);
	InteractSphere->OnComponentBeginOverlap.AddDynamic(this, &ARSTycoonNPC::OnInteract);
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

