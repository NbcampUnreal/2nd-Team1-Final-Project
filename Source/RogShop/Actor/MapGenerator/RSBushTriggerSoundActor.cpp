// Fill out your copyright notice in the Description page of Project Settings.


#include "RSBushTriggerSoundActor.h"
#include "Components/CapsuleComponent.h" 
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// Sets default values
ARSBushTriggerSoundActor::ARSBushTriggerSoundActor()
{
	PrimaryActorTick.bCanEverTick = false;

    CollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComp"));
    RootComponent = CollisionComp;

    CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
    CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    CollisionComp->InitCapsuleSize(50.f, 75.f);

    BushMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BushMesh"));
    BushMesh->SetupAttachment(RootComponent);
    BushMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    BushMesh->SetGenerateOverlapEvents(false);
}

// Called when the game starts or when spawned
void ARSBushTriggerSoundActor::BeginPlay()
{
	Super::BeginPlay();

    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ARSBushTriggerSoundActor::OnOverlapBegin);
    CollisionComp->OnComponentEndOverlap.AddDynamic(this, &ARSBushTriggerSoundActor::OnOverlapEnd);
}

void ARSBushTriggerSoundActor::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
    if (!SoundToPlay || !OtherActor)
    {
        return;
    }

    if (ACharacter* Player = Cast<ACharacter>(OtherActor))
    {
            UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, Player->GetActorLocation()); // 처음 들어왔을때 1회 재생

            OverlappingCharacter = Player;
            LastKnownPlayerLocation = Player->GetActorLocation();
            UE_LOG(LogTemp, Warning, TEXT("부쉬 재생"));

            GetWorldTimerManager().SetTimer(
                MovementCheckTimerHandle, this,
                &ARSBushTriggerSoundActor::CheckMovementAndPlaySound,
                0.25, true
            ); // 0.25초마다 플레이어의 움직임을 체크하는 타이머를 설정
    }
}

void ARSBushTriggerSoundActor::OnOverlapEnd(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex
)
{
    if (Cast<ACharacter>(OtherActor))
    {
		GetWorldTimerManager().ClearTimer(MovementCheckTimerHandle); // 타이머를 정지시킴
		OverlappingCharacter = nullptr; // 현재 범위에 있는 플레이어 캐릭터에 대한 약한 참조를 해제
    }
}

void ARSBushTriggerSoundActor::CheckMovementAndPlaySound()
{
    if (!OverlappingCharacter.IsValid())
    {
        return;
    }

    FVector CurrentLocation = OverlappingCharacter->GetActorLocation();
	float DistanceMoved = FVector::DistSquared(CurrentLocation, LastKnownPlayerLocation); // 제곱 거리를 계산

	if (DistanceMoved > FMath::Square(10)) // 플레이어가 10 이상 움직였을때
    {
		UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, GetActorLocation()); // 플레이어가 움직였을 때 소리 재생
		LastKnownPlayerLocation = CurrentLocation; // 마지막 위치 업데이트
    }
}
