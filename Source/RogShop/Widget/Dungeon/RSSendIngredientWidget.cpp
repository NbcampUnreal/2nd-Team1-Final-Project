// Fill out your copyright notice in the Description page of Project Settings.


#include "RSSendIngredientWidget.h"
#include "RogShop/UtilDefine.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "GameFramework/PlayerController.h"
#include "RSDungeonGameModeBase.h"
#include "RSGameInstance.h"
#include "RSSaveGameSubsystem.h"
#include "RSDataSubsystem.h"
#include "ItemInfoData.h"
#include "RSInventorySlotWidget.h"
#include "ItemSlot.h"
#include "RSTycoonSaveGame.h"
#include "RSDunPlayerCharacter.h"
#include "RSDungeonIngredientInventoryComponent.h"

void URSSendIngredientWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 닫기 버튼 바인딩
	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &URSSendIngredientWidget::ExitWidget);
	}

	// 확인 버튼 바인딩
	if (OKButton)
	{
		OKButton->OnClicked.AddDynamic(this, &URSSendIngredientWidget::NextStageTravel);
	}
}

void URSSendIngredientWidget::NextStageTravel()
{
	// TODO : 재료를 보낼 수 있는 만큼 골랐는지 확인 후 적게 골랐다면 경고창을 띄운다.

	URSGameInstance* GameInstance = GetGameInstance<URSGameInstance>();
	if (!GameInstance)
	{
		return;
	}

	// 타이쿤 세이브 게임으로 오브젝트 생성
	// 아이템을 타이쿤으로 세이브하기 위해서 사용
	URSTycoonSaveGame* TycoonSaveGame = Cast<URSTycoonSaveGame>(UGameplayStatics::CreateSaveGameObject(URSTycoonSaveGame::StaticClass()));
	if (!TycoonSaveGame)
	{
		return;
	}

	// 캐릭터를 통해 재료 인벤토리 컴포넌트를 가져옵니다.
	ARSDunPlayerCharacter* CurCharacter = GetOwningPlayerPawn<ARSDunPlayerCharacter>();
	if (!CurCharacter)
	{
		return;
	}

	URSDungeonIngredientInventoryComponent* IngredientInventoryComp = CurCharacter->GetRSDungeonIngredientInventoryComponent();
	if (!IngredientInventoryComp)
	{
		return;
	}

	// 선택한 재료 순회
	for (const auto & e : SendIngredientSlots)
	{
		FName ItemDataTableKey = e->GetItemDataTableKey();

		// 보낼 재료를 선택하지 않은 경우 스킵
		if (ItemDataTableKey == NAME_None)
		{
			continue;
		}

		// 보낼 재료의 개수를 가져온다.
		int32 ItemQuantity = IngredientInventoryComp->GetQuantity(ItemDataTableKey);

		// 인벤토리에서 보낼 아이템을 제거한다.
		IngredientInventoryComp->RemoveItem(ItemDataTableKey, ItemQuantity);

		// 보낼 재료를 타이쿤으로 세이브하도록 보낸다.
		//TycoonSaveGame->AddIngredient(ItemDataTableKey, ItemQuantity);
	}

	// 게임모드의 값을 다음 스테이지를 위해 변경
	ARSDungeonGameModeBase* DungeonGameMode = GetWorld()->GetAuthGameMode<ARSDungeonGameModeBase>();
	if (!DungeonGameMode)
	{
		return;
	}
	DungeonGameMode->IncrementAtTileIndex();
	DungeonGameMode->InitRandSeed();

	// 데이터 세이브 이벤트 호출
	URSSaveGameSubsystem* SaveGameSubsystem = GameInstance->GetSubsystem<URSSaveGameSubsystem>();
	if (!SaveGameSubsystem)
	{
		return;
	}

	SaveGameSubsystem->OnSaveRequested.Broadcast();

	// 다음 스테이지로 레벨 이동
	GameInstance->TravelToLevel(TargetLevelAsset);
}

void URSSendIngredientWidget::ExitWidget()
{
	// 현재 위젯을 뷰포트에서 제거하며 입력을 변경
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(false);
		PC->FlushPressedKeys();
	}

	RemoveFromParent();
}

void URSSendIngredientWidget::CreateSendIngredientSlots(int32 NumSlots)
{
	if (!SendIngredientPanel || !InvecntorySlotWidgetClass)
	{
		RS_LOG_DEBUG("SlotImageWidgetClass Null");
		return;
	}

	// 패널과 슬롯 배열 초기화
	SendIngredientPanel->ClearChildren();
	SendIngredientSlots.Empty();

	for (int32 i = 0; i < NumSlots; ++i)
	{
		// 새로운 슬롯 위젯 생성
		URSInventorySlotWidget* NewSlot = CreateWidget<URSInventorySlotWidget>(GetWorld(), InvecntorySlotWidgetClass);

		if (NewSlot)
		{
			int32 Row = 0;
			int32 Col = i;

			// 패널에 위젯을 자식으로 추가한다.
			SendIngredientPanel->AddChildToUniformGrid(NewSlot, Row, Col);
			// 슬롯 배열에 위젯을 추가한다.
			SendIngredientSlots.Add(NewSlot);

			UButton* SlotButton = NewSlot->GetSlotButton();
			if (SlotButton)
			{
				NewSlot->OnSlotClicked.AddDynamic(this, &URSSendIngredientWidget::SetClickIngredientName);
				SlotButton->OnClicked.AddDynamic(this, &URSSendIngredientWidget::SendIngredientSlotPress);
			}
		}
	}
}

void URSSendIngredientWidget::CreatePlayerIngredientSlots(const TArray<FItemSlot>& IngredientItems)
{
	if (!PlayerIngredientPanel || !InvecntorySlotWidgetClass)
	{
		RS_LOG_DEBUG("SlotImageWidgetClass Null");
		return;
	}

	PlayerIngredientPanel->ClearChildren();
	PlayerIngredientSlots.Empty();

	for (int32 i = 0; i < IngredientItems.Num(); ++i)
	{
		FItemSlot CurIngredientItemSlot = IngredientItems[i];

		FName IngredientKey = CurIngredientItemSlot.ItemKey;
		int32 ItemCount = CurIngredientItemSlot.Quantity;

		if (IngredientKey == NAME_None || ItemCount <= 0)
		{
			continue;
		}

		UWorld* CurrentWorld = GetWorld();
		if (!CurrentWorld)
		{
			return;
		}

		UGameInstance* CurrentGameInstance = CurrentWorld->GetGameInstance();
		if (!CurrentGameInstance)
		{
			return;
		}

		URSDataSubsystem* DataSubsystem = CurrentGameInstance->GetSubsystem<URSDataSubsystem>();
		if (!DataSubsystem)
		{
			return;
		}

		UDataTable* IngredientDataTable = DataSubsystem->IngredientInfo;
		if (!IngredientDataTable)
		{
			return;
		}

		FItemInfoData* IngredientInfoDataRow = IngredientDataTable->FindRow<FItemInfoData>(IngredientKey, TEXT("Get IngredientInfo"));
		if (IngredientInfoDataRow)
		{
			// 새로운 슬롯 위젯 생성
			URSInventorySlotWidget* NewSlot = CreateWidget<URSInventorySlotWidget>(GetWorld(), InvecntorySlotWidgetClass);

			if (NewSlot)
			{
				// 새롭게 생성한 위젯에 인벤토리에서 가지고 있던 재료를 표시하도록 세팅한다.
				NewSlot->SetSlotItemInfo(IngredientKey, IngredientInfoDataRow->ItemIcon, FString::FromInt(ItemCount));

				int32 Row = 0;
				int32 Col = i;

				// 패널에 위젯을 자식으로 추가한다.
				PlayerIngredientPanel->AddChildToUniformGrid(NewSlot, Row, Col);
				// 슬롯 배열에 위젯을 추가한다.
				PlayerIngredientSlots.Add(NewSlot);

				UButton* SlotButton = NewSlot->GetSlotButton();
				if (SlotButton)
				{
					NewSlot->OnSlotClicked.AddDynamic(this, &URSSendIngredientWidget::SetClickIngredientName);
					SlotButton->OnClicked.AddDynamic(this, &URSSendIngredientWidget::PlayerIngredientSlotPress);
				}
			}
		}
	}
}

void URSSendIngredientWidget::SetClickIngredientName(FName NewClickIngredientName)
{
	ClickIngredientName = NewClickIngredientName;
}

void URSSendIngredientWidget::SendIngredientSlotPress()
{
	// 클릭한 슬롯에 데이터 테이블 키값을 설정된 상태인 경우 값을 초기화한다.
	if (ClickIngredientName != NAME_None)
	{
		for (auto& e : SendIngredientSlots)
		{
			if (e->GetItemDataTableKey() == ClickIngredientName)
			{
				e->SetSlotItemInfo(NAME_None, nullptr, "");

				break;
			}
		}
	}

	ClickIngredientName = NAME_None;
}

void URSSendIngredientWidget::PlayerIngredientSlotPress()
{
	if (ClickIngredientName != NAME_None)
	{
		URSInventorySlotWidget* InventorySlotWidget = nullptr;

		FName CurItemDataTableKey;

		// 같은 키를 가진 슬롯을 찾는다.
		for (int i = 0; i < PlayerIngredientSlots.Num(); ++i)
		{
			InventorySlotWidget = PlayerIngredientSlots[i];
			if (InventorySlotWidget)
			{
				CurItemDataTableKey = InventorySlotWidget->GetItemDataTableKey();
				if (CurItemDataTableKey == ClickIngredientName)
				{
					break;
				}
			}
		}

		// 이미 존재하는지 확인한다.
		for (auto& e : SendIngredientSlots)
		{
			if (e->GetItemDataTableKey() == CurItemDataTableKey)
			{
				return;
			}
		}


		// 찾은 키를 비어있는 슬롯에 설정해줍니다.
		for (auto& e : SendIngredientSlots)
		{
			if (e->GetItemDataTableKey() == NAME_None)
			{
				UObject* IconObject = InventorySlotWidget->GetItemIcon();
				FString ItemQuantity = InventorySlotWidget->GetItemQuantity();

				e->SetSlotItemInfo(CurItemDataTableKey, IconObject, ItemQuantity);

				break;
			}
		}
	}

	ClickIngredientName = NAME_None;
}
