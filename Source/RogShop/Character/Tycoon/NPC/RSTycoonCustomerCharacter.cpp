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

void ARSTycoonCustomerCharacter::InteractTarget(AActor* TargetActor)
{
	Super::InteractTarget(TargetActor);
	
	//의자에 닿으면 앉음
	if (ARSTableTile* TableTile = Cast<ARSTableTile>(TargetActor))
	{
		TableTile->Sit(this);
	}

	//문에 닿으면 삭제
	if (ARSDoorTile* DoorTile = Cast<ARSDoorTile>(TargetActor))
	{
		Destroy();
	}
}