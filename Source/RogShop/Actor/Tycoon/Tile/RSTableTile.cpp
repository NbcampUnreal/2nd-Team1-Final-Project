// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTableTile.h"


ARSTableTile::ARSTableTile()
{
	TileName = TEXT("테이블");
	
	FoodLocation = CreateDefaultSubobject<USceneComponent>("FoodLocation");
	FoodLocation->SetupAttachment(RootComponent);
}

void ARSTableTile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARSTableTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARSTableTile::Interact()
{
	Super::Interact();

	//손님이 앉아있고 주문을 신청하면 주문 받기
	//음식을 들고 있다면 요리를 완성한 음식을 FoodLocation에 배치
	// ㄴ 배치 후 조금이따가 돈 벌리면서 손님이 사라짐
}

FVector ARSTableTile::GetFoodLocation() const
{
	return FoodLocation->GetComponentLocation();
}

