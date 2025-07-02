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
	
	if (PlayerController->IsOpenFoodListUI())
	{
		PlayerController->CloseFoodListWidget();
	}
	else
	{
		PlayerController->OpenFoodListWidget();
	}
}
