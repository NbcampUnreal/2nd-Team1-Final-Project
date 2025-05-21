// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonCustomerCharacter.h"

#include "AIController.h"
#include "RSTycoonGameModeBase.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/Actor/Tycoon/Tile/RSDoorTile.h"
#include "RogShop/Actor/Tycoon/Tile/RSTableTile.h"

// Sets default values
ARSTycoonCustomerCharacter::ARSTycoonCustomerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	InteractSphere = CreateDefaultSubobject<USphereComponent>("InteractSphere");
	InteractSphere->SetupAttachment(RootComponent);

	InteractSphere->SetCanEverAffectNavigation(false);
	InteractSphere->SetGenerateOverlapEvents(true);
	InteractSphere->SetCollisionProfileName(TEXT("OverlapAll"));
}

void ARSTycoonCustomerCharacter::MoveToTarget(FVector Location, AActor* Target)
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

	DrawDebugSphere(GetWorld(), Location, 50, 30, FColor::Red, false, 5);
	
	if (Result == EPathFollowingRequestResult::Type::Failed)
	{
		RS_LOG_C("손님 이동 실패", FColor::Red)

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

void ARSTycoonCustomerCharacter::Sit(const FTransform& SitTransform)
{
	State = ETycoonCustomerState::OrderWaiting;
	SetActorLocation(SitTransform.GetLocation());
	SetActorRotation(SitTransform.GetRotation());

	//오더는 앉자마자 추가됨
	//플레이어는 손님한테 가서 상호작용을 해서 음식을 기다리는 상태로 만들어야함	
	ARSTycoonGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>();
	GameMode->AddOrder(WantFoodKey);
}

void ARSTycoonCustomerCharacter::WaitFood()
{
	State = ETycoonCustomerState::FoodWaiting;
}

void ARSTycoonCustomerCharacter::Eat()
{
	State = ETycoonCustomerState::Eat;
	
	FTimerHandle Timer;
	GetWorldTimerManager().SetTimer(Timer, [&]()
	{
		//나감
		State = ETycoonCustomerState::Move;
		RS_LOG_C("손님이 식사를 다 했습니다", FColor::Green)

		//임시, RSTableTile에 옮길 에정
		ARSDoorTile* DoorTile = Cast<ARSDoorTile>(UGameplayStatics::GetActorOfClass(GetWorld(), ARSDoorTile::StaticClass()));
		MoveToTarget(DoorTile->GetSpawnPoint(), DoorTile);

		ARSTycoonGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>();
		GameMode->RemoveCustomer(this);

		OnLeave.Broadcast(this);
	}, 5.f, false);
}

void ARSTycoonCustomerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InteractSphere->OnComponentBeginOverlap.AddDynamic(this, &ARSTycoonCustomerCharacter::OnInteract);
}

void ARSTycoonCustomerCharacter::OnInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
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

	//의자에 닿으면 앉음
	if (ARSTableTile* TableTile = Cast<ARSTableTile>(OtherActor))
	{
		TableTile->Sit(this);
	}

	//문에 닿으면 삭제
	if (ARSDoorTile* DoorTile = Cast<ARSDoorTile>(OtherActor))
	{
		Destroy();
	}
}
