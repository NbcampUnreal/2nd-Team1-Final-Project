// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonCustomerCharacter.h"

// Sets default values
ARSTycoonCustomerCharacter::ARSTycoonCustomerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ARSTycoonCustomerCharacter::MoveToTarget()
{
	// 타겟 액터를 향해 이동
	//AIController->MoveToActor(
	//	CurrentTarget,    // 목표 액터
	//	50.0f,          // 도착 판정 반경
	//	true,           // 충돌 영역이 겹치면 도착으로 간주
	//	true,           // 경로 탐색 사용
	//	false           // 목적지를 네비게이션 메시에 투영(Projection)하지 않음
	//);
}

void ARSTycoonCustomerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}