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

float ARSTycoonCamera::GetTileMapCameraFov()
{
	ARSTileMap* TileMap = Cast<ARSTileMap>(UGameplayStatics::GetActorOfClass(GetWorld(), ARSTileMap::StaticClass()));
	check(TileMap)

	//아무래도 타일맵의 끝부분 4점을 정하고 그 점의 월드 위치를 뷰포트 위치로 투영한다음에 그 위치들이 화면안에 나올 때 까지
	//1씩 줄여야할 것 같음
	//그나마 매우 근접한 접근 방식일 것

	return 0;
}

float ARSTycoonCamera::GetCameraOrthoWidth()
{
	return 0;
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
