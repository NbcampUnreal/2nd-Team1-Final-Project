// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTileBlocker.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"


// Sets default values
ARSTileBlocker::ARSTileBlocker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetupAttachment(Mesh);
	Collision->SetCollisionProfileName("BlockAll");
	Collision->SetGenerateOverlapEvents(false);

	SetActorHiddenInGame(true); // 처음엔 숨겨진 상태
}

// Called when the game starts or when spawned
void ARSTileBlocker::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARSTileBlocker::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	FVector Loc = GetActorLocation();
	LinkedTileCoord = FIntPoint(
		FMath::RoundToInt(Loc.X / 4000.f),
		FMath::RoundToInt(Loc.Y / 4000.f)
	);
}

// Called every frame
void ARSTileBlocker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bRaising || bLowering)
	{
		AnimationTime += DeltaTime; // 애니메이션 시간 증가
		float Alpha = FMath::Clamp(AnimationTime / TotalTime, 0.f, 1.f); // 0에서 1 사이로 제한

		if (bRaising)
		{
			SetActorLocation(FMath::Lerp(HiddenLocation, ShownLocation, Alpha)); // 올라오는 위치로 보간
			Collision->SetRelativeRotation(FMath::Lerp(HiddenRotation, ShownRotation, Alpha));
			if (Alpha >= 1.f)
			{
				bRaising = false;
			}
		}
		else if (bLowering)
		{
			SetActorLocation(FMath::Lerp(ShownLocation, HiddenLocation, Alpha));
			Collision->SetRelativeRotation(FMath::Lerp(ShownRotation, HiddenRotation, Alpha));
			if (Alpha >= 1.f)
			{
				bLowering = false;
				SetActorHiddenInGame(true);
				Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
	}

}
void ARSTileBlocker::RaiseBlocker() //위로 올라오게
{
	HiddenLocation = GetActorLocation();                   // 현재 지하 위치
	ShownLocation = HiddenLocation + FVector(0, 0, 880.f); // 올라올 목표

	HiddenRotation = Collision->GetRelativeRotation();
	ShownRotation = HiddenRotation + FRotator(-90.f, 0, 0);

	SetActorLocation(HiddenLocation);  // 위치 재설정
	Collision->SetRelativeRotation(HiddenRotation);
	SetActorHiddenInGame(false);       // 보이게
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	AnimationTime = 0.f;
	bRaising = true;
	bLowering = false;
}

void ARSTileBlocker::LowerBlocker() //아래로 내려가게
{
	AnimationTime = 0.f;
	bLowering = true;
	bRaising = false;
}

FIntPoint ARSTileBlocker::GetLinkedTileCoord() const
{
	return LinkedTileCoord;
}


