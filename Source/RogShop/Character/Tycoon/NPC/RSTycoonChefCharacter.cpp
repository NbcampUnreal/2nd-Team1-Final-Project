// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonChefCharacter.h"

#include "AIController.h"
#include "RSTycoonGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/Actor/Tycoon/RSTileMap.h"
#include "RogShop/Actor/Tycoon/Tile/RSBaseTile.h"
#include "RogShop/Actor/Tycoon/Tile/RSCookingTile.h"


ARSTycoonChefCharacter::ARSTycoonChefCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARSTycoonChefCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//배치 되었을 때만 실행
	if (PlacedCookingTile == nullptr)
	{
		return;
	}

	if (bCooking)
	{
		CheckFinish();
	}
	else
	{
		//요리 가능한 상태일 때 주문이 있으면 요리 시작
		TryCook();
	}
}

void ARSTycoonChefCharacter::FindCookingTile()
{
	if (PlacedCookingTile)
	{
		return;
	}

	//쿠킹 타일 배열을 구함
	TArray<AActor*> CookingTileActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARSCookingTile::StaticClass(), CookingTileActors);

	if (CookingTileActors.Num() == 0)
	{
		RS_LOG_C("가게에 화구가 없습니다!", FColor::Red)
		return;
	}

	//가장 가까운 거리를 찾음
	ARSCookingTile* MinCookingTile = nullptr;
	int32 MinRange = INT32_MAX;

	for (auto Tile : CookingTileActors)
	{
		int32 DistanceSquared = FVector::DistSquared(GetActorLocation(), Tile->GetActorLocation());
		if (DistanceSquared < MinRange)
		{
			MinRange = DistanceSquared;
			MinCookingTile = Cast<ARSCookingTile>(Tile);
		}
	}

	//가까운 화구로 이동
	MoveToTarget(MinCookingTile->GetChefTransform().GetLocation(), MinCookingTile);
}

void ARSTycoonChefCharacter::LeaveCookingTile()
{
	PlacedCookingTile = nullptr;
}

void ARSTycoonChefCharacter::BeginPlay()
{
	Super::BeginPlay();

	FindCookingTile();
}

void ARSTycoonChefCharacter::InteractTarget(AActor* TargetActor)
{
	Super::InteractTarget(TargetActor);
	
	if (ARSCookingTile* CookingTile = Cast<ARSCookingTile>(TargetActor))
	{
		RS_LOG("요리사가 화구에 배치 되었습니다.")
		PlacedCookingTile = CookingTile;

		FTransform ChefTransform = PlacedCookingTile->GetChefTransform();
		SetActorLocation(ChefTransform.GetLocation());
		SetActorRotation(ChefTransform.GetRotation());
	}
}

void ARSTycoonChefCharacter::TryCook()
{
	if (PlacedCookingTile->GetState() != ECookingState::None)
	{
		return;
	}

	ARSTycoonGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>();
	check(GameMode)

	if (GameMode->GetOrders().Num() > 0)
	{
		bCooking = true;
		PlacedCookingTile->Interact(this);
	}
}

void ARSTycoonChefCharacter::CheckFinish()
{
	if (PlacedCookingTile->GetState() == ECookingState::Finish)
	{
		bCooking = false;
	}
}
