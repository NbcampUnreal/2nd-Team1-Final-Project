// Fill out your copyright notice in the Description page of Project Settings.

#include "RSDunBossCharacter.h"
#include "RSDungeonGameModeBase.h"
#include "Components/WidgetComponent.h"
#include "RSDunPlayerController.h"

void ARSDunBossCharacter::BeginPlay()
{
	Super::BeginPlay();

	OverheadWidget->DestroyComponent();	// 보스는 HP 바 삭제
}

float ARSDunBossCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ARSDunPlayerController* RSDunPlayerController = Cast<ARSDunPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (RSDunPlayerController)
	{
		const float HPPercent = (GetMaxHP() > 0.f) ? (float)GetHP() / (float)GetMaxHP() : 0.f;
		RSDunPlayerController->OnBossHPChange.Broadcast(GetMonsterRowName(), HPPercent);
	}

	return Damage;
}

void ARSDunBossCharacter::OnDeath()
{
	Super::OnDeath();

	ARSDungeonGameModeBase* DungeonGameMode = Cast<ARSDungeonGameModeBase>(GetWorld()->GetAuthGameMode());
	if (DungeonGameMode)
	{
		DungeonGameMode->OnBossDead.Broadcast();
	}
}

void ARSDunBossCharacter::PlaySpawnAnim()
{
	Super::PlaySpawnAnim();

	ARSDunPlayerController* PlayerController = Cast<ARSDunPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		const float HPPercent = (GetMaxHP() > 0.f) ? (float)GetHP() / (float)GetMaxHP() : 0.f;
		PlayerController->OnBossHPChange.Broadcast(GetMonsterRowName(), HPPercent);
	}
}

