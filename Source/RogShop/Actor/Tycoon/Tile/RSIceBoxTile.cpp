// Fill out your copyright notice in the Description page of Project Settings.


#include "RSIceBoxTile.h"


ARSIceBoxTile::ARSIceBoxTile()
{
	TileName = TEXT("아이스 박스");
}

void ARSIceBoxTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARSIceBoxTile::Interact()
{
	Super::Interact();

	//걍 재료 상자칸 + 될거같음
}

void ARSIceBoxTile::BeginPlay()
{
	Super::BeginPlay();
	
}
