// Fill out your copyright notice in the Description page of Project Settings.


#include "RSLightningRelic.h"
#include "RSDunPlayerCharacter.h"
#include "GameFramework/GameModeBase.h"
#include "RSSpawnManagerAccessor.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

void URSLightningRelic::ApplyEffect(ARSDunPlayerCharacter* OwnerCharacter)
{
	if (!OwnerCharacter)
	{
		return;
	}

	IRSSpawnManagerAccessor* SpawnManagerAccessor = GetWorld()->GetAuthGameMode<IRSSpawnManagerAccessor>();
	if (SpawnManagerAccessor)
	{
		URSSpawnManager* SpawnManager = SpawnManagerAccessor->GetSpawnManager();
		if (SpawnManager)
		{
			SpawnManager->OnEnemySpawn.AddDynamic(this, &URSLightningRelic::BindFunc);
		}
	}

	PlayerCharacter = Cast<ARSDunBaseCharacter>(OwnerCharacter);
}

void URSLightningRelic::LoadEffect(ARSDunPlayerCharacter* OwnerCharacter)
{
	ApplyEffect(OwnerCharacter);
}

void URSLightningRelic::BindFunc(ARSDunBaseCharacter* TargetCharacter)
{
	if (TargetCharacter)
	{
		TargetCharacter->OnCharacterHited.AddDynamic(this, &URSLightningRelic::EnemyHitEffect);
	}
}

void URSLightningRelic::EnemyHitEffect(ARSDunBaseCharacter* TargetCharacter)
{
	// 로직 실행 확률이 더 낮은 경우 실행 취소
	int32 Rand = FMath::RandRange(1, 100);
	if (Rand > Persent)
	{
		return;
	}

	if (TargetCharacter && PlayerCharacter.IsValid())
	{
		FVector SpawnLocation = TargetCharacter->GetActorLocation();
		FRotator SpawnRotation = FRotator::ZeroRotator;
		FVector Scale = FVector(1.f);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), EffectNiagara, SpawnLocation, SpawnRotation, Scale, true, true);

		FVector TraceStart = TargetCharacter->GetActorUpVector() * TargetCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		FVector TraceEnd = TargetCharacter->GetActorUpVector();

		TArray<FHitResult> HitResults;

		FCollisionQueryParams QueryParams;
		QueryParams.bTraceComplex = true;
		QueryParams.AddIgnoredActor(PlayerCharacter.Get());

		bool bHit = GetWorld()->LineTraceMultiByChannel(HitResults, TraceStart, TraceEnd, ECC_GameTraceChannel8, QueryParams);
		
		if (bHit)
		{
			for (const FHitResult& Hit : HitResults)
			{
				ARSDunBaseCharacter* HitDunCharacter = Cast<ARSDunBaseCharacter>(Hit.GetActor());
				if (HitDunCharacter)
				{
					// 피격 방향 계산
					FVector HitDirection = HitDunCharacter->GetActorLocation() - Hit.ImpactPoint;
					HitDirection.Normalize();

					UGameplayStatics::ApplyPointDamage(TargetCharacter, Amount, HitDirection, Hit, PlayerCharacter->GetController(), PlayerCharacter.Get(), UDamageType::StaticClass());
				}

			}
		}

	}

}
