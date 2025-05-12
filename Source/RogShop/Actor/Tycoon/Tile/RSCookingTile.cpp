// Fill out your copyright notice in the Description page of Project Settings.


#include "RSCookingTile.h"

#include "RogShop/UtilDefine.h"


ARSCookingTile::ARSCookingTile()
{
	TileName = TEXT("화구");
}

void ARSCookingTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARSCookingTile::Interact()
{
	Super::Interact();
	
	//주문이 들어온 요리를 제작할 수 있음
	//일단 0번째를 자동으로 요리하는식으로 제작

}

void ARSCookingTile::BeginPlay()
{
	Super::BeginPlay();
	
}
