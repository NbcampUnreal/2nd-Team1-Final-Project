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

	//가까운 순서대로 정렬
	FVector StartLocation = GetActorLocation();
	CookingTileActors.Sort([StartLocation](const AActor& First, const AActor& Second)
	{
		float FirstDistance = FVector::DistSquared(StartLocation, First.GetActorLocation());
		float SecondDistance = FVector::DistSquared(StartLocation, Second.GetActorLocation());

		return FirstDistance < SecondDistance;
	});

	//다른 쉐프 NPC가 사용하고 있는지 확인함
	ARSTycoonGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>();
	ARSCookingTile* TargetCookingTile = nullptr;
	for (AActor* Element : CookingTileActors)
	{
		ARSCookingTile* Tile = Cast<ARSCookingTile>(Element);
		bool bUse = false;

		//쉐프 중 이 타일을 한명이라도 사용 중이면 다른거에 배치함
		for (auto& NPC : GameMode->GetNPCs())
		{
			if (ARSTycoonChefCharacter* InMapChef = Cast<ARSTycoonChefCharacter>(NPC))
			{
				if (InMapChef->PlacedCookingTile == Tile)
				{
					bUse = true;
					break;
				}
			}
		}

		if (!bUse)
		{
			TargetCookingTile = Tile;
			break;
		}
	}

	//화구가 꽉 차는 등의 이유
	if (TargetCookingTile == nullptr)
	{
		RS_LOG_C("배치될 화구가 없습니다!", FColor::Red)
		return;
	}
	
	//화구로 이동하는 와중에 쉐프가 생길 수 있기 때문에 여기서 바로 설정해줌
	PlacedCookingTile = TargetCookingTile;
	MoveToTarget(TargetCookingTile->GetChefTransform().GetLocation(), TargetCookingTile);
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
