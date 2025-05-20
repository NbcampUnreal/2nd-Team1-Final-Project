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
	
	//임시
	Inventory->Add(FName(TEXT("TempGre")), 4);

	GetWorldTimerManager().SetTimerForNextTick([&]()
	{
		StartGame();
	});
}

void ARSTycoonGameModeBase::CreateCustomer()
{
	//손님의 인원이 최대 손님의 갯수를 넘을 수 없음
	if (NowCustomerCount >= MaxCustomerCount)
	{
		return;
	}

	//앉을 수 있는 테이블의 인원수보다 적거나 같은 손님을 고름
	//처음엔 최대 수로 앉을 수 있는지 검사 --> 된 손님만 추려서 손님으로 등장

	//임시, 테이블 중 랜덤으로 고름
	//현재 테이블에 따라 들어갈 수 있는 손님 설정이 안되있음
	TArray<AActor*> TableTiles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARSTableTile::StaticClass(), TableTiles);

	if (TableTiles.Num() == 0)
	{
		return;
	}

	int32 RandomTableIndex = FMath::RandRange(0, TableTiles.Num() - 1);
	ARSTableTile* TargetTableTile = Cast<ARSTableTile>(TableTiles[RandomTableIndex]);

	//손님이 들어와서 먹을 수 있는 음식이 있는지 검사
	FName OrderFoodKey;
	if (!CanOrder(OrderFoodKey))
	{
		return;
	}


	TArray<AActor*> DoorTiles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARSDoorTile::StaticClass(), DoorTiles);

	if (DoorTiles.Num() == 0)
	{
		RS_LOG("DoorTile을 찾을 수 없습니다")
		return;
	}

	int32 RandomDoorIndex = FMath::RandRange(0, DoorTiles.Num() - 1);
	ARSDoorTile* SpawnDoorTile = Cast<ARSDoorTile>(DoorTiles[RandomDoorIndex]);

	NowCustomerCount++;
	SpawnDoorTile->SpawnCustomer(OrderFoodKey, TargetTableTile);
}

//남은 재료중에 요리가 가능한지 반환
//OutOrderFood : true로 통과된다면 해야할 요리
bool ARSTycoonGameModeBase::CanOrder(FName& OutOrderFood)
{
	URSDataSubsystem* DataSubsystem = GetGameInstance()->GetSubsystem<URSDataSubsystem>();

	//1. 이미 주문 목록에 있는 음식들이 사용하는 재료를 남은 재료 목록에서 제외 시켜야함
	TMap<FName, int32> Ingredients = Inventory->GetItems(); //재료 있는 것들 복사
	for (const FName& FoodKey : OrderedFoodKeys)
	{
		FCookFoodData const* Data = DataSubsystem->Food->FindRow<FCookFoodData>(FoodKey, TEXT("Get FoodData"));
		for (auto& Need : Data->NeedIngredients)
		{
			//주문이 들어간 음식은 있는 식재료로 제작 가능한 음식들 이라는 뜻이기 때문에 딱히 Contains 검사를 해주지 않아도 됨
			Ingredients[Need.Key] -= Need.Value;
		}
	}

	//2. 남은 재료중에 만들 수 있는거 있는지 확인
	TArray<FCookFoodData*> FoodDatas;
	int OrderFoodIndex = INDEX_NONE;
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

		//테스트
		RS_LOG("Food DataTable에 등록되있는 Rows")
		for (int i = 0; i < RowNames.Num(); i++)
		{
			RS_LOG_F("%d. %s - %s", i, *RowNames[i].ToString(), *FoodDatas[i]->Name) 
		}
	}

	return bResult;
}
