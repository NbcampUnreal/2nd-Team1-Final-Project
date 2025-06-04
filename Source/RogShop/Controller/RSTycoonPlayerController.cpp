// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "RSIngredientInventoryWidget.h"
#include "RSTycoonGameModeBase.h"
#include "RSTycoonInventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/Actor/Tycoon/RSTileMap.h"
#include "RogShop/Actor/Tycoon/RSTycoonCamera.h"
#include "RogShop/Actor/Tycoon/Tile/RSBaseTile.h"
#include "RogShop/Widget/Tycoon/RSTycoonManagementWidget.h"
#include "RogShop/Widget/Tycoon/RSTycoonSaleResultWidget.h"
#include "RogShop/Widget/Tycoon/RSTycoonSaleWidget.h"
#include "RogShop/Widget/Tycoon/RSTycoonWaitWidget.h"


#pragma region Mode
ARSTycoonPlayerController::ARSTycoonPlayerController()
{
	InventoryComponent = CreateDefaultSubobject<URSTycoonInventoryComponent>(TEXT("Inventory"));
}

void ARSTycoonPlayerController::StartWaitMode()
{
	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
	ChangeMainWidget(WaitWidget);

	SetViewTargetWithBlend(MainCamera, 1.f);

	SelectTileIndex = INDEX_NONE;
}

void ARSTycoonPlayerController::StartSaleMode()
{
	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
	ChangeMainWidget(SaleWidget);
}

void ARSTycoonPlayerController::EndSaleMode()
{
	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
	ChangeMainWidget(SaleResultWidget);
}

void ARSTycoonPlayerController::StartManagementMode()
{
	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
	ChangeMainWidget(ManagementWidget);

	SetViewTargetWithBlend(TopCamera, 1.f);
}

#pragma endregion

#pragma region Widget
void ARSTycoonPlayerController::AddOrderSlot(FFoodOrder Order)
{
	SaleWidget->AddOrderSlot(Order);
}

void ARSTycoonPlayerController::RemoveOrderSlot(FFoodOrder Order)
{
	SaleWidget->RemoveOrderSlot(Order);
}

void ARSTycoonPlayerController::ActiveOrderSlot(FFoodOrder Order, FTimerHandle CookTimer)
{
	SaleWidget->StartOrderSlotAnimation(Order, CookTimer);
}

void ARSTycoonPlayerController::FinishOrderSlot(FFoodOrder Order)
{
	SaleWidget->StopOrderSlotAnimation(Order);
}

void ARSTycoonPlayerController::ChangeMainWidget(UUserWidget* ActiveWidget)
{
	WaitWidget->RemoveFromParent();
	SaleWidget->RemoveFromParent();
	SaleResultWidget->RemoveFromParent();
	ManagementWidget->RemoveFromParent();

	ActiveWidget->AddToViewport();
}

void ARSTycoonPlayerController::SettingWidget()
{
	//MainWidget
	WaitWidget = CreateWidget<URSTycoonWaitWidget>(this, WaitWidgetClass.Get());
	SaleWidget = CreateWidget<URSTycoonSaleWidget>(this, SaleWidgetClass.Get());
	SaleResultWidget = CreateWidget<URSTycoonSaleResultWidget>(this, SaleResultWidgetClass.Get());
	ManagementWidget = CreateWidget<URSTycoonManagementWidget>(this, ManagementWidgetClass.Get());

	//SubWidget
	InventoryWidget = CreateWidget<URSIngredientInventoryWidget>(this, InventoryWidgetClass.Get());
}
#pragma endregion

#pragma region Input

void ARSTycoonPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
	check(EnhancedInput)

	if (TileClickAction)
	{
		EnhancedInput->BindAction(TileClickAction, ETriggerEvent::Triggered, this, &ARSTycoonPlayerController::OnTileClick);
	}
}

void ARSTycoonPlayerController::SettingInput()
{
	check(IMC)

	auto InputSubsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	InputSubsystem->AddMappingContext(IMC, 0);
}

void ARSTycoonPlayerController::OnTileClick()
{
	ARSTycoonGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>();
	check(GameMode)

	if (GameMode->GetState() != ETycoonGameMode::Management)
	{
		return;
	}

	FHitResult HitResult;
	if (GetHitResultUnderCursor(ECC_WorldDynamic, true, HitResult))
	{
		//디버그
		DrawDebugSphere(GetWorld(), HitResult.Location, 40, 20, FColor::Blue, false, 5);

		AActor* HitActor = HitResult.GetActor();
		if (ARSBaseTile* Tile = Cast<ARSBaseTile>(HitActor))
		{
			RS_LOG_F("%s 타일이 선택됬습니다", *HitActor->GetName())

			ARSTileMap* TileMap = Cast<ARSTileMap>(UGameplayStatics::GetActorOfClass(GetWorld(), ARSTileMap::StaticClass()));
			check(TileMap)

			auto& Tiles = TileMap->GetTiles();
			for (int i = 0; i < Tiles.Num(); i++)
			{
				if (Tiles[i] == Tile)
				{
					SelectTileIndex = i;
					break;
				}
			}

			if (ManagementWidget)
			{
				ManagementWidget->BuyTileBorderSlide();
			}
			else
			{
				RS_LOG("ManagementWidget is Null")
			}
		}
	}
}

#pragma endregion

void ARSTycoonPlayerController::AddGold(int32 Value)
{
	Gold += Value;
	SaleWidget->SetGold(Gold);
}

void ARSTycoonPlayerController::AddCustomerCount(int32 Value)
{
	CustomerCount += Value;
	SaleWidget->SetCustomerCount(CustomerCount);
}

void ARSTycoonPlayerController::SetCameraLocationToCenter()
{
	ARSTileMap* TileMap = Cast<ARSTileMap>(UGameplayStatics::GetActorOfClass(GetWorld(), ARSTileMap::StaticClass()));
	check(TileMap)

	FVector Center = TileMap->GetMapCenter();
	MainCamera->SetActorLocation(Center);
	TopCamera->SetActorLocation(Center);
}

void ARSTycoonPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(true);

	SettingInput();
	SettingCamera();
	SettingWidget();
}

void ARSTycoonPlayerController::SettingCamera()
{
	TArray<AActor*> TycoonCameras;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARSTycoonCamera::StaticClass(), TycoonCameras);
	check(TycoonCameras.Num() >= 2)

	int32 MainCameraIndex =
		TycoonCameras[0]->GetComponentByClass<UCameraComponent>()->ProjectionMode == ECameraProjectionMode::Perspective ? 0 : 1;

	MainCamera = Cast<ARSTycoonCamera>(TycoonCameras[MainCameraIndex]);
	TopCamera = Cast<ARSTycoonCamera>(TycoonCameras[1 - MainCameraIndex]);
	SetViewTarget(MainCamera);

	GetWorldTimerManager().SetTimerForNextTick([&]()
	{
		SetCameraLocationToCenter();
	});
}
