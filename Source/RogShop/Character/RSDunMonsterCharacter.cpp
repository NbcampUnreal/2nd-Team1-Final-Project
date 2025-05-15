// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunMonsterCharacter.h"
#include "MeleeAttackBoxComponent.h"

ARSDunMonsterCharacter::ARSDunMonsterCharacter()
{
	MeleeAttackBoxComponent = CreateDefaultSubobject<UMeleeAttackBoxComponent>(TEXT("MeleeAttackBoxComponent"));
	MeleeAttackBoxComponent->SetupAttachment(RootComponent);

	navGenerationRadius = 500.0f;
	navRemovalRadius = 750.0f;
	jumpForce = 0.0f;

	navInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavInvoker"));	
	navInvoker->SetGenerationRadii(navGenerationRadius, navRemovalRadius);
}
/*
void ARSDunMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();

	navInvoker->SetGenerationRadii(navGenerationRadius, navRemovalRadius);
}*/

void ARSDunMonsterCharacter::PlayBaseAttackAnim()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack Complete!!"));
}

void ARSDunMonsterCharacter::PlayHitReactAnim()
{
	UE_LOG(LogTemp, Warning, TEXT("HitReact Complete!!"));
}

void ARSDunMonsterCharacter::PlayDeathAnim()
{
	UE_LOG(LogTemp, Warning, TEXT("Death Complete!!"));
}

float ARSDunMonsterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	/*if (bIsDead) return 0.0f;*/ // TODO : ���� �������� bIsDead ���� ����� �ָ� ���� ����

	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	HP -= damage;

	// ������ ������ �α׿� �߰�! (�ӽ�)
	UE_LOG(LogTemp, Warning, TEXT("[%s] took %.1f damage from [%s]"),
		*GetName(),
		damage,
		DamageCauser ? *DamageCauser->GetName() : TEXT("Unknown"));

	
	if (HP <= 0)
	{
		OnDeath();
	}

	// TODO : ���� ������� �����忡 IsHit ���� ���� �ִٰ� �ϸ� ���� ����
	/*if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BBComponent = AIController->GetBlackboardComponent())
		{
			BBComponent->SetValueAsBool(TEXT("IsHit"), true);
		}
	}*/

	return damage;
}

void ARSDunMonsterCharacter::JumpTo(FVector destination)
{
	FVector sp = GetActorLocation();
	FVector launchVelocity;
	destination.Z += jumpForce;
	bool bSuggest = UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this,
		launchVelocity,
		sp,
		destination,
		0.0f,
		0.5f
	);

	if (bSuggest)
	{
		LaunchCharacter(launchVelocity, true, true);
	}
}

void ARSDunMonsterCharacter::OnDeath()
{
	PlayDeathAnim();
}
