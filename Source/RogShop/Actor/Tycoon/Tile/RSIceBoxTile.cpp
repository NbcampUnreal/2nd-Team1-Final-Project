// Fill out your copyright notice in the Description page of Project Settings.


#include "RSIceBoxTile.h"

#include "RSTycoonInventoryComponent.h"
#include "RSTycoonPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Tycoon/RSTycoonPlayerCharacter.h"

const int32 ARSIceBoxTile::AddInventorySlot = 5;

ARSIceBoxTile::ARSIceBoxTile()
{
	TileDisplayName = TEXT("아이스 박스");
}

void ARSIceBoxTile::Interact(ACharacter* InteractCharacter)
{
	Super::Interact(InteractCharacter);

	ARSTycoonPlayerCharacter* Player = Cast<ARSTycoonPlayerCharacter>(InteractCharacter);
	if (Player == nullptr)
	{
		return;
	}

	ARSTycoonPlayerController* PlayerController = Cast<ARSTycoonPlayerController>(Player->GetController());
	check(PlayerController)

	if (PlayerController->GetInventoryComponent()->IsOpen())
	{
		check(CloseSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), CloseSound, GetActorLocation(), GetActorRotation());
		PlayerController->GetInventoryComponent()->Close();
	}
	else
	{
		check(OpenSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), OpenSound, GetActorLocation(), GetActorRotation());
		PlayerController->GetInventoryComponent()->Open();
	}
}