// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDoorTile.h"

#include "RSTableTile.h"
#include "Kismet/GameplayStatics.h"
#include "Tycoon/RSTycoonCustomerCharacter.h"


ARSDoorTile::ARSDoorTile()
{
	TileName = TEXT("문");

	CustomerSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("CustomerSpawnPoint"));
	CustomerSpawnPoint->SetupAttachment(RootComponent);
}

ARSTycoonCustomerCharacter* ARSDoorTile::SpawnCustomer(const FName& FoodKey, ARSTableTile* Target)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ARSTycoonCustomerCharacter* Customer = GetWorld()->SpawnActor<ARSTycoonCustomerCharacter>(
		CustomerType, CustomerSpawnPoint->GetComponentTransform(), SpawnParameters);
	Customer->WantFoodKey = FoodKey;

	Customer->MoveToTarget(Target->GetSitTransform().GetLocation(), Target);

	return Customer;
}
