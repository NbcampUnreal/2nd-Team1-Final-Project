// Fill out your copyright notice in the Description page of Project Settings.


#include "RogShop/GameModeBase//RSTycoonGameModeBase.h"

#include "RSTycoonInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/Actor/Tycoon/Tile/RSDoorTile.h"
#include "RogShop/Actor/Tycoon/Tile/RSTableTile.h"
#include "RogShop/DataTable/CookFoodData.h"
#include "RogShop/GameInstanceSubsystem/RSDataSubsystem.h"
#include "Tycoon/RSTycoonCustomerCharacter.h"


ARSTycoonGameModeBase::ARSTycoonGameModeBase()
{
	//어차피 1개라 성능을 많이 안 잡아 먹을거 같아서 true
	PrimaryActorTick.bCanEverTick = true;

	Inventory = CreateDefaultSubobject<URSTycoonInventoryComponent>(TEXT("Inventory"));
}

void ARSTycoonGameModeBase::StartGame()
{
	TArray<AActor*> TableTiles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARSTableTile::StaticClass(), TableTiles);

	MaxCustomerCount = 0;
	for (AActor* Element : TableTiles)
	{
		ARSTableTile* Tile = Cast<ARSTableTile>(Element);
		MaxCustomerCount += Tile->GetMaxPlace();
	}

	GetWorldTimerManager().SetTimer(CustomerTimerHandle, this, &ARSTycoonGameModeBase::CreateCustomer, 5.f, true);
}

void ARSTycoonGameModeBase::AddOrder(const FName& OrderFoodName)
{
	OrderedFoodKeys.Add(OrderFoodName);
}

void ARSTycoonGameModeBase::RemoveOrder(const FName& OrderFoodName)
{
	OrderedFoodKeys.RemoveSingle(OrderFoodName);
}

void ARSTycoonGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	
	//임시, 게임 시작 버튼 생기면 StartGame 호출하게 만들거임
	GetWorldTimerManager().SetTimerForNextTick([&]()
	{
		StartGame();
	});
}

void ARSTycoonGameModeBase::CreateCustomer()
{
	//손님의 인원이 최대 손님의 갯수를 넘을 수 없음
	if (GetCurrentCustomerCount() >= MaxCustomerCount)
	{
		return;
	}

	//손님이 들어와서 먹을 수 있는 음식이 있는지 검사
	FName OrderFoodKey;
	if (!CanOrder(OrderFoodKey))
	{
		return;
	}

	//임시, 동적 생성이 생긴 후에는 RSTileMap에서 할 예정

	//손님이 앉을 테이블 결정
	TArray<AActor*> TableTiles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARSTableTile::StaticClass(), TableTiles);
	check(TableTiles.Num())

	int32 RandomTableIndex = FMath::RandRange(0, TableTiles.Num() - 1);
	ARSTableTile* TargetTableTile = Cast<ARSTableTile>(TableTiles[RandomTableIndex]);

	//손님을 스폰할 문 결정
	TArray<AActor*> DoorTiles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARSDoorTile::StaticClass(), DoorTiles);
	check(DoorTiles.Num())

	int32 RandomDoorIndex = FMath::RandRange(0, DoorTiles.Num() - 1);
	ARSDoorTile* SpawnDoorTile = Cast<ARSDoorTile>(DoorTiles[RandomDoorIndex]);

	ARSTycoonCustomerCharacter* Customer = SpawnDoorTile->SpawnCustomer(OrderFoodKey, TargetTableTile);
	Customers.Add(Customer);
}

void ARSTycoonGameModeBase::RemoveCustomer(ARSTycoonCustomerCharacter* Customer)
{
	Customers.RemoveSingle(Customer);
}

//남은 재료중에 요리가 가능한지 반환
//OutOrderFood : true로 통과된다면 해야할 요리
bool ARSTycoonGameModeBase::CanOrder(FName& OutOrderFood)
{
	URSDataSubsystem* DataSubsystem = GetGameInstance()->GetSubsystem<URSDataSubsystem>();

	//1. 이미 들어와있는 손님들이 시키는 음식들이 사용하는 재료를 남은 재료 목록에서 제외 시켜야함
	TMap<FName, int32> Ingredients = Inventory->GetItems(); //재료 있는 것들 복사
	for (auto& Customer : Customers)
	{
		FCookFoodData const* Data = DataSubsystem->Food->FindRow<FCookFoodData>(Customer->WantFoodKey, TEXT("Get FoodData"));
		for (auto& Need : Data->NeedIngredients)
		{
			//주문이 들어간 음식은 있는 식재료로 제작 가능한 음식들 이라는 뜻이기 때문에 딱히 Contains 검사를 해주지 않아도 됨
			Ingredients[Need.Key] -= Need.Value;
		}
	}

	//2. 남은 재료중에 만들 수 있는거 있는지 확인
	TArray<FCookFoodData*> FoodDatas;
	int OrderFoodIndex = INDEX_NONE;
	FString MakeFoodName = "";
	DataSubsystem->Food->GetAllRows<FCookFoodData>(TEXT("Get All Food Data"), FoodDatas);

	for (int i = 0; i < FoodDatas.Num(); i++)
	{
		FCookFoodData* Data = FoodDatas[i];

		//만들 수 있는 음식이 1개라도 있으면 true
		if (Data->CanMake(Ingredients))
		{
			if (OrderFoodIndex == INDEX_NONE ||
				FoodDatas[OrderFoodIndex]->Price < Data->Price)
			{
				OrderFoodIndex = i;
				MakeFoodName = Data->Name;
			}
		}
	}

	bool bResult = OrderFoodIndex >= 0;
	//3. 요리를 만들 수 있다면 어떤 요리를 만들지 설정
	if (bResult)
	{
		//데이터와 Row의 대응의 순서가 보장된다면 정상작동함
		auto RowNames = DataSubsystem->Food->GetRowNames();
		OutOrderFood = RowNames[OrderFoodIndex];

		//디버그
		FCookFoodData* DebugFood = DataSubsystem->Food->FindRow<FCookFoodData>(OutOrderFood, TEXT("Debug Find Food"));
		RS_LOG_F_C("만들어야되는 음식 : %s, 지정된 Row 음식 : %s", FColor::Green, *MakeFoodName, *DebugFood->Name)
	}

	return bResult;
}
