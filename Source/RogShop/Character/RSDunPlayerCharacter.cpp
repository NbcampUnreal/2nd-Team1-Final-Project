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
#include "RSInteractable.h"
#include "DrawDebugHelpers.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

// Sets default values
ARSDunPlayerCharacter::ARSDunPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // ������ �� ������Ʈ ���� �� ��Ʈ�ѷ� ȸ���� ���� ������ �ϵ� ȸ���ϵ��� ����
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComp->SetupAttachment(RootComponent);
    SpringArmComp->bUsePawnControlRotation = true;

    // ī�޶� ������Ʈ ����
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(SpringArmComp);

    // ĳ������ �̵� �������� ȸ���ϵ��� ����
    if (UCharacterMovementComponent* CharacterMovementComp = GetCharacterMovement())
    {
        CharacterMovementComp->bOrientRotationToMovement = true;
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f); // ȸ�� �ӵ�
    }

    // ĳ���Ͱ� ��Ʈ�ѷ� Yaw�� ������ �ʵ��� ����
    bUseControllerRotationYaw = false;

    // ������ �� ��� �� ����ϴ� �ִϸ��̼� ��Ÿ��
    DodgeMontage = nullptr;
    DeathMontage = nullptr;

    // ���� ������Ʈ
    WeaponComp = CreateDefaultSubobject<URSPlayerWeaponComponent>(TEXT("RSPlayerWeapon"));

    // ��ȣ�ۿ�
    InteractActor = nullptr;
    InteractDistance = 200.f;

    // AI�ۼ��� �ڱ� �ҽ�
    AIPerceptionStimuliSourceComp = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionStimuliSource"));
    AIPerceptionStimuliSourceComp->bAutoRegister = true;
    AIPerceptionStimuliSourceComp->RegisterForSense(UAISense_Sight::StaticClass());

    // ����
    AttackPower = 0.f;
    AttackSpeed = 1.f;
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

    DecreaseHP(Damage);

    if (GetHP() <= 0)
    {
        OnDeath();
    }

    return Damage;
}

void ARSDunPlayerCharacter::OnDeath()
{
    Super::OnDeath();

    // ���� ������Ʈ�� ������ ��� ������Ʈ�� �浹���� �ʵ��� �ݸ��� ���� ����
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("DeadCharacter"));
    GetMesh()->SetCollisionProfileName(TEXT("DeadCharacter"));

    // ��� �ִϸ��̼� ���
    if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
    {
        AnimInstance->Montage_Play(DeathMontage);
    }

    // ���̻� �Է¹��� ���ϵ��� �÷��̾� ��Ʈ�ѷ��� �Է� ���� ����
    if (ARSDunPlayerController* PlayerController = Cast<ARSDunPlayerController>(GetController()))
    {
        PlayerController->RemoveAllMapping();
    }

    // ���̻� ������ �� ������ ����
    if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
    {
        MovementComponent->SetMovementMode(EMovementMode::MOVE_None);
    }

    if (AIPerceptionStimuliSourceComp)
    {
        AIPerceptionStimuliSourceComp->UnregisterFromPerceptionSystem();
    }
}

void ARSDunPlayerCharacter::Move(const FInputActionValue& value)
{
    // ī�޶� ������ �����Ͽ� �Է¹��� �������� ĳ���� �̵�
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
    // ���콺�� X, Y �������� 2D ������ ������ �������� ȸ��
    if (Controller)
    {
        FVector2D LookInput = value.Get<FVector2D>();

        // �¿� ȸ��
        AddControllerYawInput(LookInput.X);
        // ���� ȸ��
        AddControllerPitchInput(LookInput.Y);
    }
}

void ARSDunPlayerCharacter::Dodge(const FInputActionValue& value)
{
    // ���� �� �����Ⱑ ���� �ʵ����Ѵ�.
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

    // ��Ÿ�ָ� ��������� ���� ��� ������ ����
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
    // �ִϸ��̼ǰ� ��ȹ�� ���� �غ���� �ʾ����Ƿ� ������ ���
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Interaction Activated"));
    }

    IRSInteractable* Interactable = Cast<IRSInteractable>(InteractActor);
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
    // �ִϸ��̼��� ���� �غ���� �ʾ����Ƿ� ������ ���
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("StrongAttack Activated"));
    }
}

void ARSDunPlayerCharacter::FirstWeaponSlot(const FInputActionValue& value)
{
    // �ִϸ��̼��� ���� �غ���� �ʾ����Ƿ� ������ ���
    // ���Ŀ� �ش� ����� ��ȯ�Ǵ� ��� �����ؾ��Ѵ�.
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("FirstWeaponSlot Activated"));
    }

    WeaponComp->EquipWeaponToCharacter(EWeaponSlot::FirstWeaponSlot);
}

void ARSDunPlayerCharacter::SecondWeaponSlot(const FInputActionValue& value)
{
    // �ִϸ��̼��� ���� �غ���� �ʾ����Ƿ� ������ ���
    // ���Ŀ� �ش� ����� ��ȯ�Ǵ� ��� �����ؾ��Ѵ�.
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("SecondWeaponSlot Activated"));
    }

    WeaponComp->EquipWeaponToCharacter(EWeaponSlot::SecondWeaponSlot);
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
        if (HitActor && HitActor->GetClass()->ImplementsInterface(URSInteractable::StaticClass()))
        {
            IRSInteractable* Interactable = Cast<IRSInteractable>(HitActor);
            if (Interactable)
            {
                InteractActor = HitActor;

                DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 2.0f);
            }
            else
            {
                DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 2.0f);
                InteractActor = nullptr;
            }
        }
        else
        {
            DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 2.0f);
            InteractActor = nullptr;
        }
    }
    else
    {
        DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 2.0f);
        InteractActor = nullptr;
    }
}

float ARSDunPlayerCharacter::GetAttackPower() const
{
    return AttackPower;
}

void ARSDunPlayerCharacter::IncreaseAttackPower(float Amount)
{
    float NewAttackPower = AttackPower + Amount;
    AttackPower = Amount;
}

void ARSDunPlayerCharacter::DecreaseAttackPower(float Amount)
{
    float NewAttackPower = AttackPower - Amount;
    AttackPower = NewAttackPower;
}

float ARSDunPlayerCharacter::GetAttackSpeed() const
{
    return AttackSpeed;
}

void ARSDunPlayerCharacter::IncreaseAttackSpeed(float Amount)
{
    float NewAttackSpeed = AttackSpeed + Amount;
    AttackSpeed = NewAttackSpeed;
}

void ARSDunPlayerCharacter::DecreaseAttackSpeed(float Amount)
{
    // �ִϸ��̼��� ��� �ӵ��� �������� �ȴٸ� �ִϸ��̼��� ����� �ǹǷ�, ����ó��
    float NewAttackSpeed = FMath::Max(AttackSpeed - Amount, 0.0f);
    AttackSpeed = NewAttackSpeed;
}
