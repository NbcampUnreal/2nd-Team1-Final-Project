// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonCamera.h"

#include "RSTileMap.h"
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
	
	FVector Lo = SpringArm->GetRelativeLocation();
	Lo.Z *= -1;
	SetActorRelativeLocation(-Lo);
}

void ARSTycoonCamera::SetLocationToCenter()
{
	ARSTileMap* TileMap = Cast<ARSTileMap>(UGameplayStatics::GetActorOfClass(GetWorld(), ARSTileMap::StaticClass()));
	check(TileMap)
	
	FVector Center = TileMap->GetMapCenter();

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorLocation(Center);
}
