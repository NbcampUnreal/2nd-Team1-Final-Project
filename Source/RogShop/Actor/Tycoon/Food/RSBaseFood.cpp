// Fill out your copyright notice in the Description page of Project Settings.


#include "RSBaseFood.h"


ARSBaseFood::ARSBaseFood()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
}
