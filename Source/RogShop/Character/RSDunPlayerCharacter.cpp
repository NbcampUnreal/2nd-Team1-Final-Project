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
#include "RSRelicInventoryComponent.h"
#include "RSDungeonIngredientInventoryComponent.h"
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

    // 유물 컴포넌트
    RelicInventoryComp = CreateDefaultSubobject<URSRelicInventoryComponent>(TEXT("RSRelicInventory"));

    // 인벤토리 컴포넌트
    IngredientInventoryComp = CreateDefaultSubobject<URSDungeonIngredientInventoryComponent>(TEXT("IngredientInventory"));

    // 상호작용
    InteractActor = nullptr;
    InteractRadius = 200.f;

    // AI퍼셉션 자극 소스
    AIPerceptionStimuliSourceComp = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionStimuliSource"));
    AIPerceptionStimuliSourceComp->bAutoRegister = true;
    AIPerceptionStimuliSourceComp->RegisterForSense(UAISense_Sight::StaticClass());

    // 스탯
    AttackPower = 0.f;
    AttackSpeed = 1.f;
}

// Called when the game starts or when spawned
void ARSDunPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    // 스켈레탈 메시
    USkeletalMesh* MergeSkeletalMesh = USkeletalMergingLibrary::MergeMeshes(SkeletalMeshMergeParams);
    GetMesh()->SetSkeletalMeshAsset(MergeSkeletalMesh);
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

            if (PlayerController->ToggleInventoryAction)
            {
                EnhancedInput->BindAction(PlayerController->ToggleInventoryAction, ETriggerEvent::Triggered, this, &ARSDunPlayerCharacter::ToggleInventoryUI);
            }

            if (PlayerController->ToggleInGameMenuAction)
            {
                EnhancedInput->BindAction(PlayerController->ToggleInGameMenuAction, ETriggerEvent::Triggered, this, &ARSDunPlayerCharacter::ToggleInGameMenuUI);
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

    if (AIPerceptionStimuliSourceComp)
    {
        AIPerceptionStimuliSourceComp->UnregisterFromPerceptionSystem();
    }
}

void ARSDunPlayerCharacter::IncreaseMaxHP(float Amount)
{
    Super::IncreaseMaxHP(Amount);

    ARSDunPlayerController* PC = Cast<ARSDunPlayerController>(GetController());
    if (PC)
    {
        PC->OnMaxHPChange.Broadcast();
    }
}

void ARSDunPlayerCharacter::DecreaseMaxHP(float Amount)
{
    Super::DecreaseMaxHP(Amount);

    ARSDunPlayerController* PC = Cast<ARSDunPlayerController>(GetController());
    if (PC)
    {
        PC->OnMaxHPChange.Broadcast();
    }
}

void ARSDunPlayerCharacter::IncreaseHP(float Amount)
{
    Super::IncreaseHP(Amount);

    ARSDunPlayerController* PC = Cast<ARSDunPlayerController>(GetController());
    if (PC)
    {
        PC->OnHPChange.Broadcast();
    }
}

void ARSDunPlayerCharacter::DecreaseHP(float Amount)
{
    Super::DecreaseHP(Amount);

    ARSDunPlayerController* PC = Cast<ARSDunPlayerController>(GetController());
    if (PC)
    {
        PC->OnHPChange.Broadcast();
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

    WeaponComp->EquipWeaponToCharacter(EWeaponSlot::FirstWeaponSlot);
}

void ARSDunPlayerCharacter::SecondWeaponSlot(const FInputActionValue& value)
{
    // 애니메이션이 아직 준비되지 않았으므로 디버깅용 출력
    // 추후에 해당 무기로 전환되는 기능 구현해야한다.
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("SecondWeaponSlot Activated"));
    }

    WeaponComp->EquipWeaponToCharacter(EWeaponSlot::SecondWeaponSlot);
}

void ARSDunPlayerCharacter::ToggleInventoryUI(const FInputActionValue& value)
{
    // TODO : 인벤토리 UI를 켜고 끄기
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("ToggleInventoryUI Activated"));
    }
}

void ARSDunPlayerCharacter::ToggleInGameMenuUI(const FInputActionValue& value)
{
    // TODO : 다른 UI를 보이지 않도록 설정하고, 인게임 메뉴 UI를 열기
    // 현재 다른 UI는 DunMainWidget
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("ToggleInGameMenuUI Activated"));
    }
}

URSPlayerWeaponComponent* ARSDunPlayerCharacter::GetRSPlayerWeaponComponent()
{
    return WeaponComp;
}

URSDungeonIngredientInventoryComponent* ARSDunPlayerCharacter::GetRSDungeonIngredientInventoryComponent()
{
    return IngredientInventoryComp;
}

void ARSDunPlayerCharacter::InteractTrace()
{
    // 캐릭터를 중심으로 사용할 위치 선정
    FVector Center = GetActorLocation();

    // 무시할 액터, 자기 자신은 제외
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    // 오버랩된 액터를 저장할 배열
    TArray<FOverlapResult> OutOverlaps;

    // 캐릭터를 중심으로 반지름 크기의 구체를 사용해 InteractTrace 채널과 오버랩된 액터들을 탐색한다.
    bool bHit = GetWorld()->OverlapMultiByChannel(OutOverlaps, Center, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(InteractRadius), Params);
    
    if (!bHit)
    {
        InteractActor = nullptr;
        DrawDebugSphere(GetWorld(), Center, InteractRadius, 32, FColor::Red, false, 0.f, 0, 1.0f);
        return;
    }

    FVector Forward = GetActorForwardVector();

    // 전방 내에 있는 액터와 가장 가까이 있는 액터를 병행해서 계산한다.
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

        // 대상 액터까지의 벡터 및 거리 계산
        FVector ToTarget = Target->GetActorLocation() - Center;
        float Distance = ToTarget.Size();
        FVector DirToTarget = ToTarget.GetSafeNormal();

        // 전방 벡터와의 내적으로 전방과의 각도 계산
        float Dot = FVector::DotProduct(Forward, DirToTarget);
        float Angle = FMath::RadiansToDegrees(FMath::Acos(Dot));

        // 전방 내 혀용 각도 안에 있으며, 현재까지 가장 가까운 액터일 경우
        if (Angle <= InteractAngle && Distance < ClosestInFrontDist)
        {
            ClosestInFront = Target;
            ClosestInFrontDist = Distance;
        }

        // 전방 여부와 관계없이 가장 가까운 액터일 경우
        if (Distance < CosestActorDistance)
        {
            ClosestActor = Target;
            CosestActorDistance = Distance;
        }
    }

    // 전방 또는 전체 액터 중 유효한 액터가 있는 경우
    if (ClosestInFront || ClosestActor)
    {
        // 전방을 우선으로 사용한다.
        AActor* TargetActor = (ClosestInFront != nullptr) ? ClosestInFront : ClosestActor;

        // 대상 액터가 상호작용 인터페이스를 구현했는지 확인하고 구현했다면 저장한다.
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
    // 애니메이션의 재생 속도가 음수값이 된다면 애니메이션이 역재생 되므로, 예외처리
    float NewAttackSpeed = FMath::Max(AttackSpeed - Amount, 0.0f);
    AttackSpeed = NewAttackSpeed;
}
