// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonCharacter.h"

#include "EnhancedInputComponent.h"
#include "RSTycoonPlayerController.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/Actor/Tycoon/Tile/RSBaseTile.h"


ARSTycoonCharacter::ARSTycoonCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	InteractSphere = CreateDefaultSubobject<USphereComponent>("InteractSphere");
	InteractSphere->SetupAttachment(RootComponent);
}

void ARSTycoonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARSTycoonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ARSTycoonPlayerController* PlayerController = Cast<ARSTycoonPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this,
				                          &ARSTycoonCharacter::OnMove);
			}

			if (PlayerController->InteractAction)
			{
				EnhancedInput->BindAction(PlayerController->InteractAction, ETriggerEvent::Triggered, this,
				                          &ARSTycoonCharacter::OnInteract);
			}
		}
	}
}

void ARSTycoonCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ARSTycoonCharacter::OnMove(const FInputActionValue& Value)
{
	FVector Input = Value.Get<FVector>();
	
	GetMovementComponent()->AddInputVector(Input);
	
}

void ARSTycoonCharacter::OnInteract(const FInputActionValue& Value)
{
	TArray<AActor*> OverlapTileActors;
	InteractSphere->GetOverlappingActors(OverlapTileActors, ARSBaseTile::StaticClass());

	if (OverlapTileActors.Num() > 0)
	{
		AActor* MinActor = nullptr;
		int32 Min = INT32_MAX;
		FVector PlayerLocation = GetActorLocation();

		//제일 가까운 Tile 검출
		for (AActor* Tile : OverlapTileActors)
		{
			double DistanceSqrt = FVector::DistSquared2D(PlayerLocation, Tile->GetActorLocation());
			if (DistanceSqrt < Min)
			{
				Min = DistanceSqrt;
				MinActor = Tile;
			}
		}
		
		ARSBaseTile* Tile = Cast<ARSBaseTile>(MinActor);
		Tile->Interact();
	}
}
