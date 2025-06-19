// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDungeonGroundLifeEssence.h"
#include "RSDunPlayerCharacter.h"
#include "Components/CapsuleComponent.h"

ARSDungeonGroundLifeEssence::ARSDungeonGroundLifeEssence()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	InteractName = FText::FromString(TEXT("생명의 정수"));
	bIsAutoInteract = true;

	CharacterFollowSpeed = 600.f;

	InteractDelayTime = 1.5f;

	Quantity = 1;
}

void ARSDungeonGroundLifeEssence::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 상호작용 전까지 플레이어 캐릭터를 따라가도록 설정
	if (TargetActor)
	{
		FVector CurrentLocation = GetActorLocation();
		FVector TargetLocation = TargetActor->GetActorLocation();

		FVector Direction = TargetLocation - CurrentLocation;
		Direction.Normalize();

		FVector NewLocation = CurrentLocation + Direction * CharacterFollowSpeed * DeltaTime;

		SetActorLocation(NewLocation);

		// 1초에 50퍼센트씩 속도가 점점 빨라지도록 한다.
		CharacterFollowSpeed += CharacterFollowSpeed * 0.50f * DeltaTime;

		// 플레이어 캐릭터의 캡슐컴포넌트 반경만큼 가까워진 경우
		float TargetCapsuleRadius = TargetActor->GetCapsuleComponent()->GetScaledCapsuleRadius();
		float Distance = FVector::Dist(CurrentLocation, TargetLocation);

		if (Distance <= TargetCapsuleRadius)
		{
			TargetActor->IncreaseLifeEssence(Quantity);
			Destroy();
		}
	}
}

void ARSDungeonGroundLifeEssence::Interact(ARSDunPlayerCharacter* Interactor)
{
	// 더이상 상호작용 함수가 호출되지 않도록 한다.
	bIsAutoInteract = false;

	MeshComp->SetSimulatePhysics(false);
	MeshComp->SetEnableGravity(false);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetWorld()->GetTimerManager().SetTimer(InteractDelayTimer, FTimerDelegate::CreateLambda([=, this]()
	{
		// 틱 활성화
		SetActorTickEnabled(true);

		// 루트 컴포넌트를 메시 위치로 이동 및 루트 컴포넌트에 다시 부착후 메시 컴포넌트의 위치 및 회전 초기화
		SceneComp->SetWorldLocationAndRotation(MeshComp->GetComponentLocation(), MeshComp->GetComponentRotation());

		MeshComp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		MeshComp->SetRelativeLocation(FVector::ZeroVector);
		MeshComp->SetRelativeRotation(FRotator::ZeroRotator);

		// 타겟 액터 저장
		TargetActor = Interactor;
	}),
		InteractDelayTime,
		false);
}

void ARSDungeonGroundLifeEssence::SetQuantity(int32 NewQuantity)
{
	Quantity = NewQuantity;
}
