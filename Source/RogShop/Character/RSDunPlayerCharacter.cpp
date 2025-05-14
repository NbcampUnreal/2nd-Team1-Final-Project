// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "RSDunPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "RSPlayerWeaponComponent.h"
#include "RSInteractableInterface.h"
#include "DrawDebugHelpers.h"

// Sets default values
ARSDunPlayerCharacter::ARSDunPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // 스프링 암 컴포넌트 생성 및 컨트롤러 회전에 따라 스프링 암도 회전하도록 설정
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComp->SetupAttachment(RootComponent);
    SpringArmComp->bUsePawnControlRotation = true;

    // 카메라 컴포넌트 생성
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(SpringArmComp);

    // 캐릭터의 이동 방향으로 회전하도록 설정
    if (UCharacterMovementComponent* CharacterMovementComp = GetCharacterMovement())
    {
        CharacterMovementComp->bOrientRotationToMovement = true;
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f); // 회전 속도
    }

    // 캐릭터가 컨트롤러 Yaw를 따르지 않도록 설정
    bUseControllerRotationYaw = false;

    // 구르기 및 사망 시 재생하는 애니메이션 몽타주
    DodgeMontage = nullptr;
    DeathMontage = nullptr;

    // 무기 컴포넌트
    WeaponComp = CreateDefaultSubobject<URSPlayerWeaponComponent>(TEXT("RSPlayerWeapon"));

    // 상호작용
    InteractActor = nullptr;
    InteractDistance = 200.f;
}

// Called when the game starts or when spawned
void ARSDunPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARSDunPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    InteractTrace();
}

// Called to bind functionality to input
void ARSDunPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (ARSDunPlayerController* PlayerController = Cast<ARSDunPlayerController>(GetController()))
        {
            if (PlayerController->MoveAction)
            {
                EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &ARSDunPlayerCharacter::Move);
            }

            if (PlayerController->LookAction)
            {
                EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &ARSDunPlayerCharacter::Look);
            }

            if (PlayerController->DodgeAction)
            {
                EnhancedInput->BindAction(PlayerController->DodgeAction, ETriggerEvent::Triggered, this, &ARSDunPlayerCharacter::Dodge);
            }

            if (PlayerController->NormalAttackAction)
            {
                EnhancedInput->BindAction(PlayerController->NormalAttackAction, ETriggerEvent::Triggered, this, &ARSDunPlayerCharacter::NormalAttack);
            }

            if (PlayerController->StrongAttackAction)
            {
                EnhancedInput->BindAction(PlayerController->StrongAttackAction, ETriggerEvent::Triggered, this, &ARSDunPlayerCharacter::StrongAttack);
            }

            if (PlayerController->InteractionAction)
            {
                EnhancedInput->BindAction(PlayerController->InteractionAction, ETriggerEvent::Triggered, this, &ARSDunPlayerCharacter::Interaction);
            }

            if (PlayerController->FirstWeaponSlotAction)
            {
                EnhancedInput->BindAction(PlayerController->FirstWeaponSlotAction, ETriggerEvent::Triggered, this, &ARSDunPlayerCharacter::FirstWeaponSlot);
            }

            if (PlayerController->SecondWeaponSlotAction)
            {
                EnhancedInput->BindAction(PlayerController->SecondWeaponSlotAction, ETriggerEvent::Triggered, this, &ARSDunPlayerCharacter::SecondWeaponSlot);
            }
        }
    }
}

float ARSDunPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    const float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    float NewHP = FMath::Clamp(HP - Damage, 0.0f, MaxHP);

    HP = NewHP;

    if (HP <= 0)
    {
        OnDeath();
    }

    return Damage;
}

void ARSDunPlayerCharacter::OnDeath()
{
    // 레벨 오브젝트를 제외한 모든 오브젝트와 충돌하지 않도록 콜리전 설정 변경
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("DeadCharacter"));
    GetMesh()->SetCollisionProfileName(TEXT("DeadCharacter"));

    // 사망 애니메이션 재생
    if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
    {
        AnimInstance->Montage_Play(DeathMontage);
    }

    // 더이상 입력받지 못하도록 플레이어 컨트롤러의 입력 매핑 제거
    if (ARSDunPlayerController* PlayerController = Cast<ARSDunPlayerController>(GetController()))
    {
        PlayerController->RemoveAllMapping();
    }

    // 더이상 움직일 수 없도록 설정
    if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
    {
        MovementComponent->SetMovementMode(EMovementMode::MOVE_None);
    }
}

void ARSDunPlayerCharacter::Move(const FInputActionValue& value)
{
    // 카메라 시점을 기준하여 입력받은 방향으로 캐릭터 이동
    if (Controller)
    {
        const FVector2D MoveInput = value.Get<FVector2D>();
        const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

        if (MoveInput.X != 0.0f)
        {
            const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
            AddMovementInput(MovementDirection, MoveInput.X);
        }

        if (MoveInput.Y != 0.0f)
        {
            const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
            AddMovementInput(MovementDirection, MoveInput.Y);
        }
    }
}

void ARSDunPlayerCharacter::Look(const FInputActionValue& value)
{
    // 마우스의 X, Y 움직임을 2D 축으로 가져와 스프링암 회전
    if (Controller)
    {
        FVector2D LookInput = value.Get<FVector2D>();

        // 좌우 회전
        AddControllerYawInput(LookInput.X);
        // 상하 회전
        AddControllerPitchInput(LookInput.Y);
    }
}

void ARSDunPlayerCharacter::Dodge(const FInputActionValue& value)
{
    // 낙하 중 구르기가 되지 않도록한다.
    if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
    {
        if (MovementComponent->IsFalling())
        {
            return;
        }
    }

    FVector LastInput = GetLastMovementInputVector();

    if (!LastInput.IsNearlyZero())
    {
        FRotator DesiredRotation = LastInput.Rotation();
        SetActorRotation(DesiredRotation);
    }

    // 몽타주를 재생중이지 않은 경우 구르기 실행
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance && DodgeMontage)
    {
        if (!AnimInstance->IsAnyMontagePlaying())
        {
            PlayAnimMontage(DodgeMontage);
        }
    }
}

void ARSDunPlayerCharacter::Interaction(const FInputActionValue& value)
{
    // 애니메이션과 기획이 아직 준비되지 않았으므로 디버깅용 출력
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Interaction Activated"));
    }

    IRSInteractableInterface* Interactable = Cast<IRSInteractableInterface>(InteractActor);
    if (Interactable)
    {
        Interactable->Interact(this);
    }
}

void ARSDunPlayerCharacter::NormalAttack(const FInputActionValue& value)
{
    if (WeaponComp)
    {
        WeaponComp->HandleNormalAttackInput();
    }
}

void ARSDunPlayerCharacter::StrongAttack(const FInputActionValue& value)
{
    // 애니메이션이 아직 준비되지 않았으므로 디버깅용 출력
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("StrongAttack Activated"));
    }
}

void ARSDunPlayerCharacter::FirstWeaponSlot(const FInputActionValue& value)
{
    // 애니메이션이 아직 준비되지 않았으므로 디버깅용 출력
    // 추후에 해당 무기로 전환되는 기능 구현해야한다.
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("FirstWeaponSlot Activated"));
    }
}

void ARSDunPlayerCharacter::SecondWeaponSlot(const FInputActionValue& value)
{
    // 애니메이션이 아직 준비되지 않았으므로 디버깅용 출력
    // 추후에 해당 무기로 전환되는 기능 구현해야한다.
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("SecondWeaponSlot Activated"));
    }
}

URSPlayerWeaponComponent* ARSDunPlayerCharacter::GetRSPlayerWeaponComponent()
{
    return WeaponComp;
}

void ARSDunPlayerCharacter::InteractTrace()
{
    FVector Start = GetActorLocation();
    FVector ForwardVector = GetActorForwardVector();
    FVector End = Start + (ForwardVector * InteractDistance);

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_GameTraceChannel1, Params);
    
    if (bHit)
    {
        AActor* HitActor = Hit.GetActor();
        if (HitActor && HitActor->GetClass()->ImplementsInterface(URSInteractableInterface::StaticClass()))
        {
            IRSInteractableInterface* Interactable = Cast<IRSInteractableInterface>(HitActor);
            if (Interactable)
            {
                InteractActor = HitActor;

                DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 2.0f);
            }
            else
            {
                DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 2.0f);
            }
        }
        else
        {
            DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 2.0f);
        }
    }
    else
    {
        DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 2.0f);
    }
}