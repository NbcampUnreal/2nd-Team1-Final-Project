// Fill out your copyright notice in the Description page of Project Settings.

#include "RSDunBossCharacter.h"
#include "RSDungeonGameModeBase.h"
#include "Components/WidgetComponent.h"
#include "RSDunPlayerController.h" // TODO : 델리게이트 연결하면 아마 제거헤야될 듯
#include "RSBossHPBarWidget.h"
#include "RSDunPlayerController.h"

void ARSDunBossCharacter::BeginPlay()
{
	Super::BeginPlay();

	OverheadWidget->DestroyComponent();	// 보스는 HP 바 삭제
	UpdateBossName();
}

void ARSDunBossCharacter::OnDeath()
{
	Super::OnDeath();

	ARSDungeonGameModeBase* DungeonGameMode = Cast<ARSDungeonGameModeBase>(GetWorld()->GetAuthGameMode());
	if (DungeonGameMode)
	{
		DungeonGameMode->OnBossDead.Broadcast();
	}

	// TODO : 아마 선국님 델리게이트 추가하면 제거할 부분
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		ARSDunPlayerController* MyPlayerController = Cast<ARSDunPlayerController>(PlayerController);
		MyPlayerController->SetBossInfoWidgetHidden();
	}
}

void ARSDunBossCharacter::PlaySpawnAnim()
{
	Super::PlaySpawnAnim();

	// TODO : 아마 선국님 델리게이트 추가하면 제거할 부분
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		ARSDunPlayerController* MyPlayerController = Cast<ARSDunPlayerController>(PlayerController);
		MyPlayerController->SetBossInfoWidgetVisible();
	}
}

void ARSDunBossCharacter::UpdateHPUI()
{
	UpdateBossHP();
}

void ARSDunBossCharacter::UpdateBossHP()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		ARSDunPlayerController* MyPlayerController = Cast<ARSDunPlayerController>(PlayerController);

		// HPPercent = 0.0 ~ 1.0 범위의 값이 나오도록 설정
		const float HPPercent = (GetMaxHP() > 0.f) ? (float)GetHP() / (float)GetMaxHP() : 0.f;
		MyPlayerController->UpdateBossTargetPercent(HPPercent);
	}
}

void ARSDunBossCharacter::UpdateBossName()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		ARSDunPlayerController* MyPlayerController = Cast<ARSDunPlayerController>(PlayerController);
		URSBossHPBarWidget* BossInfoWidget = MyPlayerController->GetBossInfoWidget();

		BossInfoWidget->SetBossName(GetMonsterRowName());
	}
}

