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

	InteractSphere->OnComponentBeginOverlap.AddDynamic(this, &ARSTycoonCharacter::OnBeginOverlapInteract);
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

	if(OverlapTileActors.Num() > 0)
	{
		ARSBaseTile* Tile = Cast<ARSBaseTile>(OverlapTileActors[0]);
		Tile->Interact();

		RS_LOG("상호작용 됨")
	}
}

void ARSTycoonCharacter::OnBeginOverlapInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                const FHitResult& SweepResult)
{
	RS_LOG("상호작용 가능 범위 안에 들어옴")
}
