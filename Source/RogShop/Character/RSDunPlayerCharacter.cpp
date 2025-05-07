// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunPlayerCharacter.h"

// Sets default values
ARSDunPlayerCharacter::ARSDunPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARSDunPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARSDunPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARSDunPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

