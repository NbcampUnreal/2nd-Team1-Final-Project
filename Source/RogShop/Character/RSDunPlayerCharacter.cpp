// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "RSDunPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "RSPlayerWeaponComponent.h"
#include "RSInteractable.h"
#include "DrawDebugHelpers.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Engine/OverlapResult.h"

// Sets default values
ARSDunPlayerCharacter::ARSDunPlayerCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> HeadMeshAsset(TEXT("/Game/Assets/StylizedCharacters/BodyA/Heads/Head_01/SKM_HeadA_01.SKM_HeadA_01"));
    if (HeadMeshAsset.Succeeded())
    {
        HeadMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
        HeadMeshComp->SetupAttachment(GetMesh());
        HeadMeshComp->SetSkeletalMesh(HeadMeshAsset.Object);
    }

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> HairMeshAsset(TEXT("/Game/Assets/StylizedCharacters/BodyA/Hairstyles/Hairstyle_01/SKM_HairstyleA_01.SKM_HairstyleA_01"));
    if (HairMeshAsset.Succeeded())
    {
        HairMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair"));
        HairMeshComp->SetupAttachment(GetMesh());
        HairMeshComp->SetSkeletalMesh(HairMeshAsset.Object);
    }

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> EyebrowsMeshAsset(TEXT("/Game/Assets/StylizedCharacters/BodyA/Eyebrows/Eyebrows_01/SKM_EyebrowsA_01.SKM_EyebrowsA_01"));
    if (EyebrowsMeshAsset.Succeeded())
    {
        EyebrowsMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Eyebrows"));
        EyebrowsMeshComp->SetupAttachment(GetMesh());
        EyebrowsMeshComp->SetSkeletalMesh(EyebrowsMeshAsset.Object);
    }

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> MustacheMeshAsset(TEXT("/Game/Assets/StylizedCharacters/BodyA/Mustaches/Mustache_01/SKM_MustacheA_01.SKM_MustacheA_01"));
    if (MustacheMeshAsset.Succeeded())
    {
        MustacheMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mustache"));
        MustacheMeshComp->SetupAttachment(GetMesh());
        MustacheMeshComp->SetSkeletalMesh(MustacheMeshAsset.Object);
    }

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> BeardMeshAsset(TEXT("/Game/Assets/StylizedCharacters/BodyA/Beards/Beard_01/SKM_BeardA_01.SKM_BeardA_01"));
    if (BeardMeshAsset.Succeeded())
    {
        BeardMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Beard"));
        BeardMeshComp->SetupAttachment(GetMesh());
        BeardMeshComp->SetSkeletalMesh(BeardMeshAsset.Object);
    }

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> TorsoMeshAsset(TEXT("/Game/Assets/StylizedCharacters/BodyA/Body/SKM_BodyA_torso.SKM_BodyA_torso"));
    if (TorsoMeshAsset.Succeeded())
    {
        TorsoMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Torso"));
        TorsoMeshComp->SetupAttachment(GetMesh());
        TorsoMeshComp->SetSkeletalMesh(TorsoMeshAsset.Object);
    }

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> ArmsMeshAsset(TEXT("/Game/Assets/StylizedCharacters/BodyA/Body/SKM_BodyA_arms.SKM_BodyA_arms"));
    if (ArmsMeshAsset.Succeeded())
    {
        ArmsMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arms"));
        ArmsMeshComp->SetupAttachment(GetMesh());
        ArmsMeshComp->SetSkeletalMesh(ArmsMeshAsset.Object);
    }

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> HandsMeshAsset(TEXT("/Game/Assets/StylizedCharacters/BodyA/Body/SKM_BodyA_hands.SKM_BodyA_hands"));
    if (HandsMeshAsset.Succeeded())
    {
        HandsMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hands"));
        HandsMeshComp->SetupAttachment(GetMesh());
        HandsMeshComp->SetSkeletalMesh(HandsMeshAsset.Object);
    }

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> FingersMeshAsset(TEXT("/Game/Assets/StylizedCharacters/BodyA/Body/SKM_BodyA_fingers.SKM_BodyA_fingers"));
    if (FingersMeshAsset.Succeeded())
    {
        FingersMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Fingers"));
        FingersMeshComp->SetupAttachment(GetMesh());
        FingersMeshComp->SetSkeletalMesh(FingersMeshAsset.Object);
    }

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> LegsMeshAsset(TEXT("/Game/Assets/StylizedCharacters/BodyA/Body/SKM_BodyA_legs_01.SKM_BodyA_legs_01"));
    if (LegsMeshAsset.Succeeded())
    {
        LegsMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs"));
        LegsMeshComp->SetupAttachment(GetMesh());
        LegsMeshComp->SetSkeletalMesh(LegsMeshAsset.Object);
    }

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> FeetMeshAsset(TEXT("/Game/Assets/StylizedCharacters/BodyA/Body/SKM_BodyA_feet.SKM_BodyA_feet"));
    if (FeetMeshAsset.Succeeded())
    {
        FeetMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Feet"));
        FeetMeshComp->SetupAttachment(GetMesh());
        FeetMeshComp->SetSkeletalMesh(FeetMeshAsset.Object);
    }

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> TopArmorMeshAsset(TEXT("/Game/Assets/AdventurersKit/BodyA/Armor_01/SKM_BodyA_Armor_01_Top.SKM_BodyA_Armor_01_Top"));
    if (TopArmorMeshAsset.Succeeded())
    {
        TopArmorMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TopArmor"));
        TopArmorMeshComp->SetupAttachment(GetMesh());
        TopArmorMeshComp->SetSkeletalMesh(TopArmorMeshAsset.Object);
    }

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> GlovesMeshAsset(TEXT("/Game/Assets/AdventurersKit/BodyA/Armor_01/SKM_BodyA_Armor_01_Gloves.SKM_BodyA_Armor_01_Gloves"));
    if (GlovesMeshAsset.Succeeded())
    {
        GlovesMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gloves"));
        GlovesMeshComp->SetupAttachment(GetMesh());
        GlovesMeshComp->SetSkeletalMesh(GlovesMeshAsset.Object);
    }

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> PantsMeshAsset(TEXT("/Game/Assets/AdventurersKit/BodyA/Armor_01/SKM_BodyA_Armor_01_Pants.SKM_BodyA_Armor_01_Pants"));
    if (PantsMeshAsset.Succeeded())
    {
        PantsMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pants"));
        PantsMeshComp->SetupAttachment(GetMesh());
        PantsMeshComp->SetSkeletalMesh(PantsMeshAsset.Object);
    }

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> BootsMeshAsset(TEXT("/Game/Assets/AdventurersKit/BodyA/Armor_01/SKM_BodyA_Armor_01_Boots.SKM_BodyA_Armor_01_Boots"));
    if (BootsMeshAsset.Succeeded())
    {
        BootsMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Boots"));
        BootsMeshComp->SetupAttachment(GetMesh());
        BootsMeshComp->SetSkeletalMesh(BootsMeshAsset.Object);
    }



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
    InteractRadius = 200.f;

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

    UE_LOG(LogTemp, Warning, TEXT("[%s] took %.1f damage from [%s], Current Hp is %f"),
        *GetName(),
        Damage,
        DamageCauser ? *DamageCauser->GetName() : TEXT("Unknown"),
        GetHP()
    );

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
    // ĳ���͸� �߽����� ����� ��ġ ����
    FVector Center = GetActorLocation();

    // ������ ����, �ڱ� �ڽ��� ����
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    // �������� ���͸� ������ �迭
    TArray<FOverlapResult> OutOverlaps;

    // ĳ���͸� �߽����� ������ ũ���� ��ü�� ����� InteractTrace ä�ΰ� �������� ���͵��� Ž���Ѵ�.
    bool bHit = GetWorld()->OverlapMultiByChannel(OutOverlaps, Center, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(InteractRadius), Params);
    
    if (!bHit)
    {
        InteractActor = nullptr;
        DrawDebugSphere(GetWorld(), Center, InteractRadius, 32, FColor::Red, false, 0.f, 0, 1.0f);
        return;
    }

    FVector Forward = GetActorForwardVector();

    // ���� ���� �ִ� ���Ϳ� ���� ������ �ִ� ���͸� �����ؼ� ����Ѵ�.
    AActor* ClosestInFront = nullptr;
    float ClosestInFrontDist = TNumericLimits<float>::Max();

    AActor* ClosestActor = nullptr;
    float CosestActorDistance = TNumericLimits<float>::Max();

    for (const FOverlapResult& Result : OutOverlaps)
    {
        AActor* Target = Result.GetActor();
        if (!Target)
        {
            continue;
        }

        // ��� ���ͱ����� ���� �� �Ÿ� ���
        FVector ToTarget = Target->GetActorLocation() - Center;
        float Distance = ToTarget.Size();
        FVector DirToTarget = ToTarget.GetSafeNormal();

        // ���� ���Ϳ��� �������� ������� ���� ���
        float Dot = FVector::DotProduct(Forward, DirToTarget);
        float Angle = FMath::RadiansToDegrees(FMath::Acos(Dot));

        // ���� �� ��� ���� �ȿ� ������, ������� ���� ����� ������ ���
        if (Angle <= InteractAngle && Distance < ClosestInFrontDist)
        {
            ClosestInFront = Target;
            ClosestInFrontDist = Distance;
        }

        // ���� ���ο� ������� ���� ����� ������ ���
        if (Distance < CosestActorDistance)
        {
            ClosestActor = Target;
            CosestActorDistance = Distance;
        }
    }

    // ���� �Ǵ� ��ü ���� �� ��ȿ�� ���Ͱ� �ִ� ���
    if (ClosestInFront || ClosestActor)
    {
        // ������ �켱���� ����Ѵ�.
        AActor* TargetActor = (ClosestInFront != nullptr) ? ClosestInFront : ClosestActor;

        // ��� ���Ͱ� ��ȣ�ۿ� �������̽��� �����ߴ��� Ȯ���ϰ� �����ߴٸ� �����Ѵ�.
        if (TargetActor && TargetActor->GetClass()->ImplementsInterface(URSInteractable::StaticClass()))
        {
            IRSInteractable* Interactable = Cast<IRSInteractable>(TargetActor);
            if (Interactable)
            {
                InteractActor = TargetActor;

                DrawDebugSphere(GetWorld(), Center, InteractRadius, 32, FColor::Green, false, 0.f, 0, 1.0f);
            }
            else
            {
                DrawDebugSphere(GetWorld(), Center, InteractRadius, 32, FColor::Red, false, 0.f, 0, 1.0f);
            }
        }
        else
        {
            DrawDebugSphere(GetWorld(), Center, InteractRadius, 32, FColor::Red, false, 0.f, 0, 1.0f);
        }
    }
    else
    {
        DrawDebugSphere(GetWorld(), Center, InteractRadius, 32, FColor::Red, false, 0.f, 0, 1.0f);
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
