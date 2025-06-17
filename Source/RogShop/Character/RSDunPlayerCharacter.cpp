// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDunPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "RSDunPlayerController.h"
#include "RSDunMonsterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "RSPlayerWeaponComponent.h"
#include "RSRelicInventoryComponent.h"
#include "RSDungeonIngredientInventoryComponent.h"
#include "RSInteractable.h"
#include "DrawDebugHelpers.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Engine/OverlapResult.h"
#include "RSSaveGameSubsystem.h"
#include "RSDungeonStatusSaveGame.h"
#include "RogShop/UtilDefine.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"


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

    // 플레이어 캐릭터가 가지는 애니메이션 몽타주 및 관련 변수
    DodgeMontage = nullptr;
    DeathMontage = nullptr;

    bIsMontageSkippable = true;

    // 던전 재화
    LifeEssence = 0;

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

    UGameInstance* CurGameInstance = GetGameInstance();
    if (!CurGameInstance)
    {
        return;
    }

    URSSaveGameSubsystem* SaveGameSubsystem = CurGameInstance->GetSubsystem<URSSaveGameSubsystem>();
    if (!SaveGameSubsystem)
    {
        return;
    }

    SaveGameSubsystem->OnSaveRequested.AddDynamic(this, &ARSDunPlayerCharacter::SaveStatus);

    LoadStatus();
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

    RS_LOG_DEBUG("[%s] took %.1f damage from [%s], Current Hp is %f",
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

    // AI에게 감지되지 않도록 AI퍼셉션 자극 소스 제거
    if (AIPerceptionStimuliSourceComp)
    {
        AIPerceptionStimuliSourceComp->UnregisterFromPerceptionSystem();
    }

    // 던전 세이브 파일 제거
    UGameInstance* CurGameInstance = GetGameInstance();
    if (CurGameInstance)
    {
        URSSaveGameSubsystem* SaveGameSubsystem = CurGameInstance->GetSubsystem<URSSaveGameSubsystem>();
        if (SaveGameSubsystem)
        {
            SaveGameSubsystem->DeleteDungeonSaveFile();
        }
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
        PC->OnFloatDamage.Broadcast(Amount);
    }
}

void ARSDunPlayerCharacter::Move(const FInputActionValue& value)
{
    // 카메라 시점을 기준하여 입력받은 방향으로 캐릭터 이동
    if (Controller && TrySkipMontage())
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

    // 구르기 전 몬스터 HP바가 플레이어를 바라보도록 하기
    // 1. 모든 몬스터 위젯 컴포넌트 회전
    for (ARSDunMonsterCharacter* Monster : ARSDunMonsterCharacter::GetAllMonsters())
    {
        if (IsValid(Monster))
        {
            Monster->UpdateEnemyWidgetComponentRotation();
        }
    }

    if (TrySkipMontage())
    {
        FVector LastInput = GetLastMovementInputVector();

        if (!LastInput.IsNearlyZero())
        {
            FRotator DesiredRotation = LastInput.Rotation();
            SetActorRotation(DesiredRotation);
        }

        PlayAnimMontage(DodgeMontage);
    }
}

void ARSDunPlayerCharacter::Interaction(const FInputActionValue& value)
{
    IRSInteractable* Interactable = Cast<IRSInteractable>(InteractActor);
    if (Interactable)
    {
        Interactable->Interact(this);

        UAnimMontage* InteractAnimMontage = Interactable->GetInteractAnimMontage();

        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

        if (InteractAnimMontage && AnimInstance && !AnimInstance->IsAnyMontagePlaying())
        {
            PlayAnimMontage(InteractAnimMontage);
        }
    }
}

void ARSDunPlayerCharacter::NormalAttack(const FInputActionValue& value)
{
    if (WeaponComp)
    {
        WeaponComp->HandleAttackInput(EAttackType::NormalAttack);
    }
}

void ARSDunPlayerCharacter::StrongAttack(const FInputActionValue& value)
{
    if (WeaponComp)
    {
        WeaponComp->HandleAttackInput(EAttackType::StrongAttack);
    }
}

void ARSDunPlayerCharacter::FirstWeaponSlot(const FInputActionValue& value)
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (!AnimInstance)
    {
        return;
    }

    // 무기 교체 애니메이션을 재생 중이지 않으며, 스킵이 가능한 경우
    if (!AnimInstance->Montage_IsPlaying(ChangeWeaponMontage) && TrySkipMontage())
    {
        // 타겟 슬롯을 변경
        WeaponComp->SetChangeTargetSlot(EWeaponSlot::FirstWeaponSlot);

        // 변경한 타겟 슬롯으로 무기 교체가 가능한지 확인
        if (WeaponComp->CanChangeTargetSlot())
        {
            // 무기 교체 몽타주 재생
            PlayAnimMontage(ChangeWeaponMontage);
        }
    }
}

void ARSDunPlayerCharacter::SecondWeaponSlot(const FInputActionValue& value)
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (!AnimInstance)
    {
        return;
    }

    // 무기 교체 애니메이션을 재생 중이지 않으며, 스킵이 가능한 경우
    if (!AnimInstance->Montage_IsPlaying(ChangeWeaponMontage) && TrySkipMontage())
    {
        // 타겟 슬롯을 변경
        WeaponComp->SetChangeTargetSlot(EWeaponSlot::SecondWeaponSlot);

        // 변경한 타겟 슬롯으로 무기 교체가 가능한지 확인
        if (WeaponComp->CanChangeTargetSlot())
        {
            // 무기 교체 몽타주 재생
            PlayAnimMontage(ChangeWeaponMontage);
        }
    }
}

void ARSDunPlayerCharacter::ToggleInventoryUI(const FInputActionValue& value)
{
    // TODO : 인벤토리 UI를 켜고 끄기
    ARSDunPlayerController* PC = GetController<ARSDunPlayerController>();
    if (PC)
    {
        PC->TogglePlayerInventoryWidget();
    }

    RS_LOG_DEBUG("ToggleInventoryUI Activated");
}

void ARSDunPlayerCharacter::ToggleInGameMenuUI(const FInputActionValue& value)
{
    // 인게임 메뉴 UI를 보이도록 하거나 안보이도록 한다.
    ARSDunPlayerController* PC = GetController<ARSDunPlayerController>();
    if (PC)
    {
        PC->ToggleInGameMenuWidget();
    }

    RS_LOG_DEBUG("ToggleInGameMenuUI Activated");
}

void ARSDunPlayerCharacter::SetIsMontageSkippable(bool NewbIsMontageSkippable)
{
    bIsMontageSkippable = NewbIsMontageSkippable;
}

bool ARSDunPlayerCharacter::IsSkippingMontage() const
{
    return bIsSkippingMontage;
}

bool ARSDunPlayerCharacter::TrySkipMontage()
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

    // 애니메이션 인스턴스가 유효하고 스킵 가능한 상태인 경우
    if (AnimInstance && bIsMontageSkippable)
    {
        // 상체만 재생하는 몽타주를 제외한 다른 몽타주를 재생 중인 경우
        float UpperBodyLocalWeight = AnimInstance->GetSlotMontageLocalWeight(FName(TEXT("UpperBody")));
        bool IsUpperBodySlotActive = UpperBodyLocalWeight > 0;
        if (!IsUpperBodySlotActive && AnimInstance->IsAnyMontagePlaying())
        {
            UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();

            if (CurrentMontage)
            {
                // 스킵 로직 시작
                bIsSkippingMontage = true;

                // 현재 몽타주의 재생 시간을 가져온다.
                float CurrentPostion = AnimInstance->Montage_GetPosition(CurrentMontage);
                // 현재 몽타주의 총 재생 시간을 가져온다.
                float CurrentPlayLength = CurrentMontage->GetPlayLength();
                // 현재 재생 시간으로부터 총 재생 시간까지 남은 시간을 구한다.
                float DeltaTime = CurrentPlayLength - CurrentPostion;

                // 현재 재생 시간으로부터 DeltaTime까지의 위치에 존재하는 노티파이를 모두 가져온다.
                TArray<const FAnimNotifyEvent*> SkippedNotifies;
                CurrentMontage->GetAnimNotifies(CurrentPostion, DeltaTime, false, SkippedNotifies);

                // 노티파이를 순회한다.
                for (const FAnimNotifyEvent* NotifyState : SkippedNotifies)
                {
                    // 노티파이 스테이트인 경우에만 NotifyEnd를 호출한다.
                    if (NotifyState && NotifyState->NotifyStateClass)
                    {
                        NotifyState->NotifyStateClass->NotifyEnd(GetMesh(), CurrentMontage);
                    }
                }

                // 몽타주를 스킵한다.
                AnimInstance->Montage_Stop(0.f, CurrentMontage);

                // 스킵 로직 끝
                bIsSkippingMontage = false;

                // 몽타주를 스킵한 경우
                return true;
            }
        }
        else
        {
            // 몽타주 재생 중이 아닌 상태다.
            return true;
        }
    }

    return false;
}

void ARSDunPlayerCharacter::SetLifeEssence(int32 Amount)
{
    int32 NewLifeEssence = FMath::Max(Amount, 0.0f);
    LifeEssence = NewLifeEssence;

    ARSDunPlayerController* PC = Cast<ARSDunPlayerController>(GetController());
    if (PC)
    {
        PC->OnLifeEssenceChange.Broadcast(LifeEssence);
    }
}

void ARSDunPlayerCharacter::IncreaseLifeEssence(int32 Amount)
{
    int32 NewLifeEssence = LifeEssence + Amount;
    LifeEssence = NewLifeEssence;

    ARSDunPlayerController* PC = Cast<ARSDunPlayerController>(GetController());
    if (PC)
    {
        PC->OnLifeEssenceChange.Broadcast(LifeEssence);
    }
}

void ARSDunPlayerCharacter::DecreaseLifeEssence(int32 Amount)
{
    int32 NewLifeEssence = FMath::Max(LifeEssence - Amount, 0.0f);
    LifeEssence = NewLifeEssence;

    ARSDunPlayerController* PC = Cast<ARSDunPlayerController>(GetController());
    if (PC)
    {
        PC->OnLifeEssenceChange.Broadcast(LifeEssence);
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
    
    if (bHit)
    {
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

                    // 바로 상호작용 해야하는 경우
                    if (Interactable->GetIsAutoInteract() == true)
                    {
                        Interactable->Interact(this);
                    }
                    // 유저가 직접 상호작용 해야하는 경우
                    else
                    {
                        ARSDunPlayerController* PC = Cast<ARSDunPlayerController>(GetController());
                        if (PC)
                        {
                            PC->ShowInteractWidget();
                            PC->OnInteractableFound.Broadcast(Interactable->GetInteractName());
                        }
                    }

                    RS_DRAW_DEBUG_SPHERE(GetWorld(), Center, InteractRadius, 32, FColor::Green, false, 0.f, 0, 1.0f);

                    return;
                }
            }
        }
    }

    // 상호작용 가능한 액터를 찾지 못한 경우
    InteractActor = nullptr;

    ARSDunPlayerController* PC = Cast<ARSDunPlayerController>(GetController());
    if (PC)
    {
        PC->HideInteractWidget();
    }

    RS_DRAW_DEBUG_SPHERE(GetWorld(), Center, InteractRadius, 32, FColor::Red, false, 0.f, 0, 1.0f);
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

void ARSDunPlayerCharacter::SaveStatus()
{
    // SaveGame 오브젝트 생성
    URSDungeonStatusSaveGame* StatusSaveGame = Cast<URSDungeonStatusSaveGame>(UGameplayStatics::CreateSaveGameObject(URSDungeonStatusSaveGame::StaticClass()));
    if (!StatusSaveGame)
    {
        return;
    }

    StatusSaveGame->HP = GetHP();
    StatusSaveGame->LifeEssence = LifeEssence;

    // 저장
    UGameInstance* CurGameInstance = GetGameInstance();
    if (!CurGameInstance)
    {
        return;
    }

    URSSaveGameSubsystem* SaveGameSubsystem = CurGameInstance->GetSubsystem<URSSaveGameSubsystem>();
    if (!SaveGameSubsystem)
    {
        return;
    }

    UGameplayStatics::SaveGameToSlot(StatusSaveGame, SaveGameSubsystem->StatusSaveSlotName, 0);
}

void ARSDunPlayerCharacter::LoadStatus()
{
    // 저장된 세이브 로드
    UGameInstance* CurGameInstance = GetGameInstance();
    if (!CurGameInstance)
    {
        return;
    }

    URSSaveGameSubsystem* SaveGameSubsystem = CurGameInstance->GetSubsystem<URSSaveGameSubsystem>();
    if (!SaveGameSubsystem)
    {
        return;
    }

    URSDungeonStatusSaveGame* StatusLoadGame = Cast<URSDungeonStatusSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameSubsystem->StatusSaveSlotName, 0));
    if (!StatusLoadGame)
    {
        return;
    }
    
    // 로드
    ChangeHP(StatusLoadGame->HP);
    LifeEssence = StatusLoadGame->LifeEssence;
}
USoundBase* ARSDunPlayerCharacter::GetFootstepSound(EPhysicalSurface SurfaceType) const
{
    const USoundBase* const* FoundSound = FootstepSounds.Find(SurfaceType);
    return FoundSound ? const_cast<USoundBase*>(*FoundSound) : nullptr;
}