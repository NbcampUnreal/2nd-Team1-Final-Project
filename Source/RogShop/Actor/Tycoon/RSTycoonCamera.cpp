// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonCamera.h"

#include "RSTileMap.h"
#include "RSTycoonPlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RogShop/UtilDefine.h"
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
	if (Target.IsValid())
	{
		return;
	}

	ARSTycoonPlayerCharacter* Player = Cast<ARSTycoonPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	check(Player)
	Target = Player;

	PlusVec = -SpringArm->GetRelativeLocation();
	PlusVec.Z += AttachPlusZ;
}

void ARSTycoonCamera::SetLocationToCenter()
{
	ARSTileMap* TileMap = Cast<ARSTileMap>(UGameplayStatics::GetActorOfClass(GetWorld(), ARSTileMap::StaticClass()));
	check(TileMap)

	Target = nullptr;
	FVector Center = TileMap->GetMapCenter();

	SetActorLocation(Center);
}

void ARSTycoonCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Target == nullptr)
	{
		return;
	}

	SetActorLocation(Target->GetActorLocation() + PlusVec);
}
