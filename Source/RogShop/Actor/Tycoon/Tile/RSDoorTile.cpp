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

void ARSDoorTile::SpawnCustomer()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	ARSTycoonCustomerCharacter* Customer = GetWorld()->SpawnActor<ARSTycoonCustomerCharacter>(CustomerType, SpawnParameters);
	Customer->SetActorLocation(CustomerSpawnPoint->GetComponentLocation());

	//임시
	Customer->WantFoodName = TEXT("TempFood");

	ARSTableTile* TableTile = Cast<ARSTableTile>(UGameplayStatics::GetActorOfClass(GetWorld(), ARSTableTile::StaticClass()));
	TableTile->Sit({Customer});
}