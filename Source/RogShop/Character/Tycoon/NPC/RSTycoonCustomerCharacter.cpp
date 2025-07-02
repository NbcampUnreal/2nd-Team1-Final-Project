// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonCustomerCharacter.h"

#include "AIController.h"
#include "CookFoodData.h"
#include "RSDataSubsystem.h"
#include "RSTycoonGameModeBase.h"
#include "RSTycoonPlayerController.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/Actor/Tycoon/Tile/RSDoorTile.h"
#include "RogShop/Actor/Tycoon/Tile/RSTableTile.h"
#include "RogShop/Widget/Tycoon/RSTycoonFoodBubbleWidget.h"

// Sets default values
ARSTycoonCustomerCharacter::ARSTycoonCustomerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	FoodBubbleWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("FoodBubble"));
	FoodBubbleWidgetComponent->SetupAttachment(RootComponent);
}

void ARSTycoonCustomerCharacter::Sit(ARSTableTile* Table, const FTransform& SitTransform)
{
	State = ETycoonCustomerState::OrderWaiting;
	SitTableTile = Table;

	SetActorLocation(SitTransform.GetLocation());
	SetActorRotation(SitTransform.GetRotation());
	
	FoodBubbleWidgetComponent->SetVisibility(true);
	URSTycoonFoodBubbleWidget* BubbleWidget = Cast<URSTycoonFoodBubbleWidget>(FoodBubbleWidgetComponent->GetWidget());
	check(BubbleWidget)
	BubbleWidget->SetImage(WantFoodKey);

	PlayAnimMontage(SitMontage);
}

void ARSTycoonCustomerCharacter::WaitFood()
{
	State = ETycoonCustomerState::FoodWaiting;

	FFoodOrder Order;
	Order.FoodKey = WantFoodKey;
	Order.Customer = this;

	ARSTycoonGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>();
	GameMode->AddOrder(Order);
}

void ARSTycoonCustomerCharacter::Eat()
{
	State = ETycoonCustomerState::Eat;

	FoodBubbleWidgetComponent->SetVisibility(false);

	FTimerHandle Timer;
	GetWorldTimerManager().SetTimer(Timer, this, &ARSTycoonCustomerCharacter::Leave, 5.f, false);
}

void ARSTycoonCustomerCharacter::StopAllAction()
{
	Super::StopAllAction();

	GetWorldTimerManager().ClearAllTimersForObject(this);
	
	State = ETycoonCustomerState::Move;
	SitTableTile = nullptr;
}

void ARSTycoonCustomerCharacter::BeginPlay()
{
	Super::BeginPlay();

	FoodBubbleWidgetComponent->SetVisibility(false);

	// 커스텀 스켈레탈 메시 적용
	USkeletalMesh* MergeSkeletalMesh = USkeletalMergingLibrary::MergeMeshes(SkeletalMeshMergeParams);
	GetMesh()->SetSkeletalMeshAsset(MergeSkeletalMesh);
}

void ARSTycoonCustomerCharacter::InteractTarget(AActor* TargetActor)
{
	Super::InteractTarget(TargetActor);

	//의자에 닿으면 앉음
	if (ARSTableTile* TableTile = Cast<ARSTableTile>(TargetActor))
	{
		TableTile->Sit(this);
	}

	//문에 닿으면 삭제
	if (ARSDoorTile* DoorTile = Cast<ARSDoorTile>(TargetActor))
	{
		ARSTycoonGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>();
		GameMode->RemoveCustomer(this);

		Destroy();
	}
}

void ARSTycoonCustomerCharacter::Leave()
{
	State = ETycoonCustomerState::Move;
	FCookFoodData* Data = GetGameInstance()->GetSubsystem<URSDataSubsystem>()->Food->
	                                         FindRow<FCookFoodData>(WantFoodKey, TEXT("Get Price Of Food"));

	RS_LOG_F_C("손님이 식사를 다 했습니다, 골드 +%d", FColor::Yellow, Data->Price);

	ARSTycoonPlayerController* PlayerController = GetWorld()->GetFirstPlayerController<ARSTycoonPlayerController>();
	PlayerController->AddGold(Data->Price);
	PlayerController->AddCustomerCount(1);
	
	//[추가], 돈 추가/감소 이펙트 (이펙트 안에 있는 숫자가 Value로 변경)
	PlayerController->FloatingGold(Data->Price, GetActorLocation());

	OnFinishEat.Broadcast(this);

	float PlayTime = PlayAnimMontage(StandMontage);
	FTimerHandle StandTimer;
	GetWorldTimerManager().SetTimer(StandTimer, this, &ARSTycoonCustomerCharacter::MoveToDoor, PlayTime, false);
}

void ARSTycoonCustomerCharacter::MoveToDoor()
{
	ARSDoorTile* DoorTile = Cast<ARSDoorTile>(UGameplayStatics::GetActorOfClass(GetWorld(), ARSDoorTile::StaticClass()));
	MoveToTarget(DoorTile->GetSpawnPoint(), DoorTile);
}
