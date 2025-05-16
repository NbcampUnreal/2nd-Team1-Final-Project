// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAttackBoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RSDunPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UMeleeAttackBoxComponent::UMeleeAttackBoxComponent()
{
	// �ݸ��� ����
	SetCollisionEnabled(ECollisionEnabled::NoCollision); // �⺻�� ���ΰ�, ���� Ÿ�ֿ̹� �浹 �����ѱ�
	SetCollisionObjectType(ECC_WorldDynamic);
	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SetGenerateOverlapEvents(true);

}

void UMeleeAttackBoxComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UMeleeAttackBoxComponent::OnAttackOverlap);

}

void UMeleeAttackBoxComponent::OnAttackOverlap(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	// ���� ���ݿ� �������� ��ü�� �ڱ� �ڽ��̰ų� ������ ���� X
	if (!OtherActor || OtherActor == GetOwner())
	{
		return;
	}

	// �� ���� ������Ʈ�� �������� ĳ���Ϳ� ������ ����
	UGameplayStatics::ApplyDamage(
		OtherActor,
		10.f,
		GetOwner()->GetInstigatorController(),
		GetOwner(),
		UDamageType::StaticClass()
	);

	// �˹� ó��
	if (ARSDunPlayerCharacter* HitCharacter = Cast<ARSDunPlayerCharacter>(OtherActor))
	{
		UCharacterMovementComponent* MoveComp = HitCharacter->GetCharacterMovement();
	
		FVector KnockbackDir = (OtherActor->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal();
		FVector KnockbackForce = KnockbackDir * 5000.f;

		MoveComp->AddImpulse(KnockbackForce, true);
		UE_LOG(LogTemp, Warning, TEXT("KnockBack Success!!"));
	}

	// Overlap �׽�Ʈ�� �α�
	UE_LOG(LogTemp, Warning, TEXT("Hit actor: %s"), *OtherActor->GetName());

}
