// Fill out your copyright notice in the Description page of Project Settings.


#include "RSFoodListTableTile.h"

#include "RSTycoonPlayerController.h"


// Sets default values
ARSFoodListTableTile::ARSFoodListTableTile()
{
	TileDisplayName = TEXT("레시피 테이블");
}

void ARSFoodListTableTile::Interact(ACharacter* InteractCharacter)
{
	Super::Interact(InteractCharacter);

	ARSTycoonPlayerController* PlayerController = GetWorld()->GetFirstPlayerController<ARSTycoonPlayerController>();
	check(PlayerController);
	
	if (bUIOpen)
	{
		bUIOpen = false;
		PlayerController->CloseFoodListWidget();
	}
	else
	{
		bUIOpen = true;
		PlayerController->OpenFoodListWidget();
	}
}
