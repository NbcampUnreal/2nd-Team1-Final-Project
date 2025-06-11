// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDungeonGroundLifeEssence.h"
#include "RSDunPlayerCharacter.h"

ARSDungeonGroundLifeEssence::ARSDungeonGroundLifeEssence()
{
	InteractName = FText::FromString(TEXT("생명의 정수"));
	bIsAutoInteract = true;

	CharacterFollowTime = 0.1f;
	InteractDelayTime = 1.f;

	CharacterFollowSpeed = 300.f;

	Quantity = 1;
}

void ARSDungeonGroundLifeEssence::Interact(ARSDunPlayerCharacter* Interactor)
{
	bIsAutoInteract = false;

	MeshComp->SetSimulatePhysics(false);
	MeshComp->SetEnableGravity(false);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 상호작용 전까지 플레이어 캐릭터를 따라가도록 설정
	GetWorld()->GetTimerManager().SetTimer(CharacterFollowTimer, FTimerDelegate::CreateLambda([=, this]()
		{
			if (Interactor)
			{
				FVector CurrentLocation = GetActorLocation();

				FVector Direction = Interactor->GetActorLocation() - CurrentLocation;
				Direction.Normalize();

				FVector NewLocation = CurrentLocation + Direction * CharacterFollowSpeed * CharacterFollowTime;
			}
		}),
		CharacterFollowTime,
		true);

	// 상호작용
	GetWorld()->GetTimerManager().SetTimer(InteractDelayTimer, FTimerDelegate::CreateLambda([=, this]()
		{
			if (Interactor)
			{
				Interactor->IncreaseLifeEssence(Quantity);
			}

			Destroy();
		}),
		InteractDelayTime,
		false);
}

void ARSDungeonGroundLifeEssence::SetQuantity(int32 NewQuantity)
{
	Quantity = NewQuantity;
}
