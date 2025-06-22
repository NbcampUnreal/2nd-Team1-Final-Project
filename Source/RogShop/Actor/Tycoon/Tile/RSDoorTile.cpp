// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDoorTile.h"

#include "RSTableTile.h"
#include "Kismet/GameplayStatics.h"
#include "Tycoon/NPC/RSTycoonCustomerCharacter.h"


ARSDoorTile::ARSDoorTile()
{
	TileDisplayName = TEXT("문");

	CustomerSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("CustomerSpawnPoint"));
	CustomerSpawnPoint->SetupAttachment(RootComponent);
}

ARSTycoonCustomerCharacter* ARSDoorTile::SpawnCustomer(const FName& FoodKey, ARSTableTile* TargetTable)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ARSTycoonCustomerCharacter* Customer = GetWorld()->SpawnActor<ARSTycoonCustomerCharacter>(
		CustomerType, CustomerSpawnPoint->GetComponentTransform(), SpawnParameters);
	Customer->WantFoodKey = FoodKey;

	int32 Index = TargetTable->GetCanSitingLocationIndex();
	Customer->MoveToTarget(TargetTable->GetSitTransform(Index).GetLocation(), TargetTable);	

	return Customer;
}
