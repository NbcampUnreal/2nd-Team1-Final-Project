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

	// TODO : AIController를 만들어 주시면 합치고 나서 이거 주석 풀어야됨.

	// 타겟 액터를 향해 이동
	//AIController->MoveToActor(
	//	CurrentTarget,    // 목표 액터
	//	50.0f,          // 도착 판정 반경
	//	true,           // 충돌 영역이 겹치면 도착으로 간주
	//	true,           // 경로 탐색 사용
	//	false           // 목적지를 네비게이션 메시에 투영(Projection)하지 않음
	//);

	UE_LOG(LogTemp, Display, TEXT("%s moving to target: %s"),
		*GetName(), *CurrentTarget->GetName());
}

