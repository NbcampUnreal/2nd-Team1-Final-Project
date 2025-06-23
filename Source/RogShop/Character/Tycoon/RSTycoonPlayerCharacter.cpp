// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "RSTycoonGameModeBase.h"
#include "RSTycoonPlayerController.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/Actor/Tycoon/Tile/RSBaseTile.h"
#include "RogShop/Actor/Tycoon/Tile/RSCookingTile.h"
#include "RogShop/Actor/Tycoon/Tile/RSTableTile.h"


ARSTycoonPlayerCharacter::ARSTycoonPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	InteractSphere = CreateDefaultSubobject<USphereComponent>("InteractSphere");
	InteractSphere->SetupAttachment(RootComponent);

	PickupLocation = CreateDefaultSubobject<USceneComponent>("FoodLocation");
	PickupLocation->SetupAttachment(GetMesh(), TEXT("PickupSocket"));

	// 컨트롤러 회전 사용 비활성화
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// Character Movement Component 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
}

void ARSTycoonPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	check(EnhancedInput)

	ARSTycoonPlayerController* PlayerController = Cast<ARSTycoonPlayerController>(GetController());
	check(PlayerController)
	
	if (PlayerController->MoveAction)
	{
		EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this,
		                          &ARSTycoonPlayerCharacter::OnMove);
	}

	if (PlayerController->InteractAction)
	{
		EnhancedInput->BindAction(PlayerController->InteractAction, ETriggerEvent::Triggered, this,
		                          &ARSTycoonPlayerCharacter::OnInteract);
	}
}

void ARSTycoonPlayerCharacter::Pickup(AActor* Actor)
{
	if (PickupActor)
	{
		RS_LOG_C("이미 들고있는 음식이 있습니다.", FColor::Red)
		return;
	}
	
	PickupActor = Actor;
	
	Actor->AttachToComponent(PickupLocation, FAttachmentTransformRules::KeepWorldTransform);
	Actor->SetActorRelativeTransform(FTransform::Identity);

	check(PickupFoodMontage)
	PlayAnimMontage(PickupFoodMontage);
}

AActor* ARSTycoonPlayerCharacter::Drop(FTransform DropTransform)
{
	if (PickupActor == nullptr)
	{
		RS_LOG_C("들고있는 Actor가 없습니다", FColor::Red);
		return nullptr;
	}

	AActor* Temp = PickupActor.Get();
	
	PickupActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	PickupActor->SetActorTransform(DropTransform);
	PickupActor = nullptr;

	check(DropFoodMontage)
	PlayAnimMontage(DropFoodMontage);
	
	return Temp;
}

void ARSTycoonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 커스텀 스켈레탈 메시 적용
	USkeletalMesh* MergeSkeletalMesh = USkeletalMergingLibrary::MergeMeshes(SkeletalMeshMergeParams);
	GetMesh()->SetSkeletalMeshAsset(MergeSkeletalMesh);
}

void ARSTycoonPlayerCharacter::PlayInteractAnimation(ARSBaseTile* InteractTile)
{
	//바빠잇

	ARSTycoonGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>();
	check(GameMode)
	
	if (ARSTableTile* TableTile = Cast<ARSTableTile>(InteractTile))
	{
		if (TableTile->GetOrderWaitCustomerIndex() != INDEX_NONE)
		{
			check(TakeOrderMontage)
			PlayAnimMontage(TakeOrderMontage);
		}
	}

	if (ARSCookingTile* CookingTile = Cast<ARSCookingTile>(InteractTile))
	{
		if (CookingTile->GetState() == ECookingState::None &&
			GameMode->GetOrders().Num() > 0)
		{
			bCooking = true;
			SetActorTransform(CookingTile->GetChefTransform());
		}
	}
}

void ARSTycoonPlayerCharacter::OnMove(const FInputActionValue& Value)
{
	if (bCooking)
	{
		return;
	}
	
	FVector Input = Value.Get<FVector>();
	GetMovementComponent()->AddInputVector(Input);
}

void ARSTycoonPlayerCharacter::OnInteract(const FInputActionValue& Value)
{
	if (bCooking)
	{
		return;
	}
	
	TArray<AActor*> OverlapTileActors;
	InteractSphere->GetOverlappingActors(OverlapTileActors, ARSBaseTile::StaticClass());

	if (OverlapTileActors.Num() == 0)
	{
		return;
	}
	
	AActor* MinActor = nullptr;
	int32 Min = INT32_MAX;
	const FVector PlayerLocation = GetActorLocation();

	//제일 가까운 Tile 검출
	for (AActor* Tile : OverlapTileActors)
	{
		double DistanceSqrt = FVector::DistSquared2D(PlayerLocation, Tile->GetActorLocation());
		if (DistanceSqrt < Min)
		{
			Min = DistanceSqrt;
			MinActor = Tile;
		}
	}

	ARSBaseTile* Tile = Cast<ARSBaseTile>(MinActor);
	RS_LOG_F("상호작용 하는 Tile 이름 : %s", *Tile->GetTileDisplayName());

	PlayInteractAnimation(Tile);
	Tile->Interact(this);
}
