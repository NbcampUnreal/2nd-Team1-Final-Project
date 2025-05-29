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

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
}

void ARSTycoonCamera::AttachPlayer()
{
	ARSTycoonPlayerCharacter* Player = Cast<ARSTycoonPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	check(Player)

	AttachToActor(Player, FAttachmentTransformRules::KeepWorldTransform);
	SetActorRelativeLocation(FVector::ZeroVector);
}

void ARSTycoonCamera::ReturnToOrigin()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorTransform(OriginTransform);
}

void ARSTycoonCamera::BeginPlay()
{
	Super::BeginPlay();

	OriginTransform = GetActorTransform();
}
