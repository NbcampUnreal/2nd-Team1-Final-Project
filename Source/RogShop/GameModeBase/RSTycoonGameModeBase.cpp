// Fill out your copyright notice in the Description page of Project Settings.


#include "RogShop/GameModeBase//RSTycoonGameModeBase.h"

#include "RSTycoonInventoryComponent.h"
#include "RSTycoonPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/Actor/Tycoon/Tile/RSDoorTile.h"
#include "RogShop/Actor/Tycoon/Tile/RSTableTile.h"
#include "RogShop/DataTable/CookFoodData.h"
#include "RogShop/GameInstanceSubsystem/RSDataSubsystem.h"
#include "Tycoon/NPC/RSTycoonCustomerCharacter.h"
#include "ItemSlot.h"
#include "RSIngredientInventoryWidget.h"
#include "RSSaveGameSubsystem.h"
#include "RSTycoonSaveGame.h"
#include "RogShop/Actor/Tycoon/RSTileMap.h"
#include "RogShop/Actor/Tycoon/Tile/RSIceBoxTile.h"


ARSTycoonGameModeBase::ARSTycoonGameModeBase()
{
	//어차피 1개라 성능을 많이 안 잡아 먹을거 같아서 true
	PrimaryActorTick.bCanEverTick = true;
}

void ARSTycoonGameModeBase::StartSaleMode()
{
	State = ETycoonGameMode::Sale;

	SetMaxCustomerCount();

	ARSTycoonPlayerController* Controller = GetWorld()->GetFirstPlayerController<ARSTycoonPlayerController>();
	check(Controller)
	Controller->StartSaleMode();

	GetWorldTimerManager().SetTimer(CustomerTimerHandle, this, &ARSTycoonGameModeBase::CreateCustomer, 5.f, true);
	GetWorldTimerManager().SetTimer(GameTimerHandle, this, &ARSTycoonGameModeBase::EndSaleMode, SalePlayMinute * 60, false);
}

void ARSTycoonGameModeBase::SetMaxCustomerCount()
{
	//들어올 수 있는 최대 손님 갯수 설정 
	TArray<AActor*> TableTiles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARSTableTile::StaticClass(), TableTiles);

	MaxCustomerCount = 0;
	for (AActor* Element : TableTiles)
	{
		ARSTableTile* Tile = Cast<ARSTableTile>(Element);
		MaxCustomerCount += Tile->GetMaxPlace();
	}
}

void ARSTycoonGameModeBase::AddOrder(FFoodOrder Order)
{
	FoodOrders.Add(Order);

	ARSTycoonPlayerController* Controller = GetWorld()->GetFirstPlayerController<ARSTycoonPlayerController>();
	check(Controller)
	Controller->AddOrderSlot(Order);
}

void ARSTycoonGameModeBase::RemoveOrder(FFoodOrder Order)
{
	FoodOrders.RemoveSingle(Order);
}

void ARSTycoonGameModeBase::RemoveCustomer(ARSTycoonCustomerCharacter* Customer)
{
	Customers.RemoveSingle(Customer);

	//게임 종료
	FName OrderFoodKey;
	if (Customers.Num() == 0 && !CanOrder(OrderFoodKey))
	{
		EndSaleMode();
	}
}

void ARSTycoonGameModeBase::AddNPC(ARSTycoonNPC* NPC)
{
	//손님은 추가하지 않음. GameMode에서 생성을 전반해서 맡아야하기 때문
	if (!NPC->IsA<ARSTycoonCustomerCharacter>())
	{
		NPCs.Add(NPC);
	}
}

void ARSTycoonGameModeBase::RemoveNPC(ARSTycoonNPC* NPC)
{
	if (!NPC->IsA<ARSTycoonCustomerCharacter>())
	{
		NPCs.RemoveSingle(NPC);
	}
}

void ARSTycoonGameModeBase::EndSaleMode()
{
	RS_LOG_C("게임 끝", FColor::Orange);

	State = ETycoonGameMode::EndSale;

	//리셋
	GetWorldTimerManager().ClearTimer(CustomerTimerHandle);
	GetWorldTimerManager().ClearTimer(GameTimerHandle);

	for (auto& NPC : NPCs)
	{
		NPC->StopAllAction();
	}

	for (auto& Customer : Customers)
	{
		Customer->StopAllAction();
		Customer->Destroy();
	}

	Customers.Empty();
	FoodOrders.Empty();

	ARSTileMap* TileMap = Cast<ARSTileMap>(UGameplayStatics::GetActorOfClass(GetWorld(), ARSTileMap::StaticClass()));
	check(TileMap)
	TileMap->ResetAllTile();

	GetWorld()->GetFirstPlayerController<ARSTycoonPlayerController>()->EndSaleMode();
}

void ARSTycoonGameModeBase::StartWaitMode()
{
	State = ETycoonGameMode::Wait;

	GetWorld()->GetFirstPlayerController<ARSTycoonPlayerController>()->StartWaitMode();
}

void ARSTycoonGameModeBase::StartManagementMode()
{
	State = ETycoonGameMode::Management;

	GetWorld()->GetFirstPlayerController<ARSTycoonPlayerController>()->StartManagementMode();
}

float ARSTycoonGameModeBase::GetGameTime() const
{
	return GetWorldTimerManager().GetTimerElapsed(GameTimerHandle);
}

FFoodOrder ARSTycoonGameModeBase::GetOrderToCook()
{
	//제일 처음 주문을 반환
	return FoodOrders[0];
}

void ARSTycoonGameModeBase::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITOR
	GetGameInstance<URSGameInstance>()->SetDebugLogEnabled(true);
#endif

	URSSaveGameSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<URSSaveGameSubsystem>();
	SaveSubsystem->OnSaveRequested.AddDynamic(this, &ARSTycoonGameModeBase::SaveGameData);

	GetWorldTimerManager().SetTimerForNextTick([&]()
	{
		LoadGameData();
		StartWaitMode();
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

	ARSTableTile* TargetTableTile;
	do
	{
		int32 RandomTableIndex = FMath::RandRange(0, TableTiles.Num() - 1);
		TargetTableTile = Cast<ARSTableTile>(TableTiles[RandomTableIndex]);
	} while (!TargetTableTile->CanSit());

	//손님을 스폰할 문 결정
	TArray<AActor*> DoorTiles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARSDoorTile::StaticClass(), DoorTiles);
	check(DoorTiles.Num())

	int32 RandomDoorIndex = FMath::RandRange(0, DoorTiles.Num() - 1);
	ARSDoorTile* SpawnDoorTile = Cast<ARSDoorTile>(DoorTiles[RandomDoorIndex]);

	ARSTycoonCustomerCharacter* Customer = SpawnDoorTile->SpawnCustomer(OrderFoodKey, TargetTableTile);
	Customers.Add(Customer);

	if (CustomerAddSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(
			this,
			CustomerAddSound,
			SpawnDoorTile->GetActorLocation(),
			FRotator::ZeroRotator
		);
	}
}

//남은 재료중에 요리가 가능한지 반환
//OutOrderFood : true로 통과된다면 해야할 요리
bool ARSTycoonGameModeBase::CanOrder(FName& OutOrderFood)
{
	URSDataSubsystem* DataSubsystem = GetGameInstance()->GetSubsystem<URSDataSubsystem>();
	ARSTycoonPlayerController* Controller = GetWorld()->GetFirstPlayerController<ARSTycoonPlayerController>();
	check(Controller)

	//1. 이미 들어와있는 손님들이 시키는 음식들이 사용하는 재료를 남은 재료 목록에서 제외 시켜야함
	TArray<FItemSlot> Ingredients = Controller->GetInventoryComponent()->GetItems(); //재료 있는 것들 복사
	for (auto& Customer : Customers)
	{
		FCookFoodData const* Data = DataSubsystem->Food->FindRow<FCookFoodData>(Customer->WantFoodKey, TEXT("Get FoodData"));
		for (auto& Need : Data->NeedIngredients)
		{
			//손님이 바라는 음식인데 이미 요리가 나와서 재료가 없는 상황엔 재료를 차감하지 않음
			//어차피 아래쪽에서 만들 수 있는지 검사하기 때문에 여기서 차감만
			for (FItemSlot& e : Ingredients)
			{
				if (e.ItemKey == Need.Key)
				{
					e.Quantity -= Need.Value;
				}
			}
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

	//3. 요리를 만들 수 있다면 어떤 요리를 만들지 설정
	bool bResult = OrderFoodIndex >= 0;
	if (bResult)
	{
		//데이터와 Row의 대응의 순서가 보장된다면 정상작동함
		auto RowNames = DataSubsystem->Food->GetRowNames();
		OutOrderFood = RowNames[OrderFoodIndex];
	}

	return bResult;
}

void ARSTycoonGameModeBase::LoadGameData()
{
	ARSTycoonPlayerController* Controller = GetWorld()->GetFirstPlayerController<ARSTycoonPlayerController>();
	check(Controller)

	URSSaveGameSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<URSSaveGameSubsystem>();
	FString TycoonSaveSlot = SaveSubsystem->TycoonSaveSlot;
	URSTycoonSaveGame* SaveGame = Cast<URSTycoonSaveGame>(UGameplayStatics::LoadGameFromSlot(TycoonSaveSlot, 0));

	if (SaveGame == nullptr)
	{
		//최초 파일 생성
		SaveGame = Cast<URSTycoonSaveGame>(UGameplayStatics::CreateSaveGameObject(URSTycoonSaveGame::StaticClass()));
		SaveGame->SetDefault();
		
		//최초 파일 저장
		UGameplayStatics::SaveGameToSlot(SaveGame, TycoonSaveSlot, 0);
	}
	
	Controller->GetInventoryComponent()->LoadItemData();
	Controller->GetInventoryComponent()->UpdateInventoryWidget();
	Controller->SetGold(SaveGame->Money);
}

void ARSTycoonGameModeBase::SaveGameData()
{
	ARSTycoonPlayerController* Controller = GetWorld()->GetFirstPlayerController<ARSTycoonPlayerController>();
	check(Controller)

	FString SaveSlot = GetGameInstance()->GetSubsystem<URSSaveGameSubsystem>()->TycoonSaveSlot;
	URSTycoonSaveGame* SaveGame =
		Cast<URSTycoonSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlot, 0));

	SaveGame->Money = Controller->GetGold();

	UGameplayStatics::SaveGameToSlot(SaveGame, SaveSlot, 0);
}
