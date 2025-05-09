// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonBaseAICharacter.h"

// Sets default values
ARSTycoonBaseAICharacter::ARSTycoonBaseAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARSTycoonBaseAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARSTycoonBaseAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARSTycoonBaseAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARSTycoonBaseAICharacter::MoveToTarget()
{
	if (!CurrentTarget)
	{
		UE_LOG(LogTemp, Warning, TEXT("MoveToTarget failed: No Target Actor set for %s"), *GetName());
		return;
	}

	// TODO : AIController�� ����� �ֽø� ��ġ�� ���� �̰� �ּ� Ǯ��ߵ�.

	// Ÿ�� ���͸� ���� �̵�
	//AIController->MoveToActor(
	//	CurrentTarget,    // ��ǥ ����
	//	50.0f,          // ���� ���� �ݰ�
	//	true,           // �浹 ������ ��ġ�� �������� ����
	//	true,           // ��� Ž�� ���
	//	false           // �������� �׺���̼� �޽ÿ� ����(Projection)���� ����
	//);

	UE_LOG(LogTemp, Display, TEXT("%s moving to target: %s"),
		*GetName(), *CurrentTarget->GetName());
}

