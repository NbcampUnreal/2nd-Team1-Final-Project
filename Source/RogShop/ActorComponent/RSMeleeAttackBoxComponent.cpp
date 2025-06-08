// Fill out your copyright notice in the Description page of Project Settings.


#include "RSMeleeAttackBoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RSDunPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RogShop/UtilDefine.h"

URSMeleeAttackBoxComponent::URSMeleeAttackBoxComponent()
{
	// 콜리전 설정
	SetCollisionEnabled(ECollisionEnabled::NoCollision); // 기본은 꺼두고, 공격 타이밍에 충돌 설정켜기
	SetCollisionObjectType(ECC_WorldDynamic);
	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SetGenerateOverlapEvents(true);

}

void URSMeleeAttackBoxComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &URSMeleeAttackBoxComponent::OnAttackOverlap);

}

void URSMeleeAttackBoxComponent::OnAttackOverlap(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	// 만약 공격에 오버랩된 물체가 자기 자신이거나 없으면 실행 X
	if (!OtherActor || OtherActor == GetOwner())
	{
		return;
	}

	// 이 공격 컴포넌트에 오버랩된 캐릭터에 데미지 적용
	UGameplayStatics::ApplyDamage(
		OtherActor,
		10.f,
		GetOwner()->GetInstigatorController(),
		GetOwner(),
		UDamageType::StaticClass()
	);

	// 넉백 처리
	if (ARSDunPlayerCharacter* HitCharacter = Cast<ARSDunPlayerCharacter>(OtherActor))
	{
		UCharacterMovementComponent* MoveComp = HitCharacter->GetCharacterMovement();
	
		FVector KnockbackDir = (OtherActor->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal();
		FVector KnockbackForce = KnockbackDir * 5000.f;

		MoveComp->AddImpulse(KnockbackForce, true);
		RS_LOG_DEBUG("KnockBack Success!!");
	}

	// Overlap 테스트용 로그
	RS_LOG_DEBUG("Hit actor: %s", *OtherActor->GetName());
}
