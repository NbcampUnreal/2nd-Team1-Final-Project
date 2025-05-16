// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonCamera.h"

#include "RSTycoonPlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Tycoon/RSTycoonPlayerCharacter.h"


ARSTycoonCamera::ARSTycoonCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);
	
	Camera = CreateDefaultSubobject<UCameraComponent>("MainCamera");
	Camera->SetupAttachment(SpringArm);

	TopCamera = CreateDefaultSubobject<UCameraComponent>("TopCamera");
	TopCamera->SetupAttachment(RootComponent);
}

void ARSTycoonCamera::Attach()
{
	ARSTycoonPlayerCharacter* Player = Cast<ARSTycoonPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	check(Player)

	AttachToActor(Player, FAttachmentTransformRules::KeepWorldTransform);
	SetActorRelativeLocation(FVector::ZeroVector);
}

void ARSTycoonCamera::BeginPlay()
{
	Super::BeginPlay();

	if (ARSTycoonPlayerController* PlayerController = Cast<ARSTycoonPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		PlayerController->SetViewTarget(this);
	}
}

