// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "RSIngredientInventoryWidget.h"
#include "RSTycoonGameModeBase.h"
#include "RSTycoonInventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/Actor/Tycoon/RSTileMap.h"
#include "RogShop/Actor/Tycoon/RSTycoonCamera.h"
#include "RogShop/Actor/Tycoon/Tile/RSBaseTile.h"
#include "RogShop/Object/RSTycoonEvent.h"
#include "RogShop/Widget/Tycoon/RSTycoonEventViewWidget.h"
#include "RogShop/Widget/Tycoon/RSTycoonManagementWidget.h"
#include "RogShop/Widget/Tycoon/RSTycoonNPCInfoWidget.h"
#include "RogShop/Widget/Tycoon/RSTycoonSaleResultWidget.h"
#include "RogShop/Widget/Tycoon/RSTycoonSaleWidget.h"
#include "RogShop/Widget/Tycoon/RSTycoonWaitWidget.h"
#include "Tycoon/NPC/RSTycoonNPC.h"
#include "RSPlayerFloatingDamageWidget.h"
#include "RogShop/Actor/Tycoon/Tile/RSIceBoxTile.h"
#include "RogShop/Widget/Tycoon/RSTycoonCameraChangeWidget.h"
#include "RogShop/Widget/Tycoon/RSTycoonFoodListWidget.h"
#include "Tycoon/RSTycoonPlayerCharacter.h"

#pragma region Mode
ARSTycoonPlayerController::ARSTycoonPlayerController()
{
	InventoryComponent = CreateDefaultSubobject<URSTycoonInventoryComponent>(TEXT("Inventory"));
}

void ARSTycoonPlayerController::StartWaitMode(bool bNoAnimation)
{
	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
	ChangeMainWidget(WaitWidget, bNoAnimation);

	DisableSelectTileOutline();

	//인벤토리 크기 설정
	TArray<AActor*> IceBoxActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARSIceBoxTile::StaticClass(), IceBoxActors);

	InventoryComponent->SetMaxSlot(ARSIceBoxTile::AddInventorySlot * (IceBoxActors.Num() + 1));
	InventoryComponent->UpdateInventoryWidget();

	//리미트 검사
	CheckLimitsOfSale();
}

void ARSTycoonPlayerController::StartSaleMode()
{
	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
	ChangeMainWidget(SaleWidget);

	SaleGold = 0;
	CustomerCount = 0;
	SaleWidget->SetCustomerCount(CustomerCount);
}

void ARSTycoonPlayerController::EndSaleMode()
{
	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
	ChangeMainWidget(SaleResultWidget);

	SaleWidget->RemoveAllOrderSlots();

	ARSTycoonPlayerCharacter* PlayerCharacter = Cast<ARSTycoonPlayerCharacter>(GetCharacter());
	PlayerCharacter->ResetPickup();
	PlayerCharacter->SetCooking(false);
}

void ARSTycoonPlayerController::StartManagementMode()
{
	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
	ChangeMainWidget(ManagementWidget);
}

void ARSTycoonPlayerController::SetSaleEnable(bool Value)
{
	WaitWidget->SetEnableSaleButton(Value);
}

bool ARSTycoonPlayerController::CheckLimitsOfSale()
{
	UWorld* World = GetWorld();

	//이벤트 검사
	for (auto& Event : LimitsForSale)
	{
		URSTycoonEvent* EventObject = Event->GetDefaultObject<URSTycoonEvent>();
		if (EventObject->Condition(World))
		{
			EventObject->Success(World);
			EventViewWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			EventObject->Fail(World);

			EventViewWidget->SetVisibility(ESlateVisibility::Visible);
			EventViewWidget->Set(EventObject);
			return false;
		}
	}

	return true;
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

void ARSTycoonPlayerController::OpenFoodListWidget()
{
	bIsOpenFoodListUI = true;
	FoodListWidget->AddToViewport();
}

void ARSTycoonPlayerController::CloseFoodListWidget()
{
	bIsOpenFoodListUI = false;
	FoodListWidget->RemoveFromParent();
}

void ARSTycoonPlayerController::ChangeMainWidget(UUserWidget* ActiveWidget, bool bNoAnimation)
{
	auto Lamda = [&, ActiveWidget]()
	{
		WaitWidget->RemoveFromParent();
		SaleWidget->RemoveFromParent();
		SaleResultWidget->RemoveFromParent();
		ManagementWidget->RemoveFromParent();

		ActiveWidget->AddToViewport();

		if (ActiveWidget == ManagementWidget)
		{
			SetViewTarget(TopCamera);
			TopCamera->SetLocationToCenter();
		}
		else if (ActiveWidget == WaitWidget || ActiveWidget == SaleWidget)
		{
			SetViewTarget(MainCamera);
		}
	};

	if (bNoAnimation)
	{
		Lamda();
	}
	else
	{
		CameraChangeWidget->Active(Lamda);
	}
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
	NPCInfoWidget = CreateWidget<URSTycoonNPCInfoWidget>(this, NPCInfoWidgetClass.Get());

	FloatingTextWidget = CreateWidget<URSPlayerFloatingDamageWidget>(this, FloatingTextWidgetClass);
	FloatingTextWidget->SetTextColor(FColor::Yellow);

	EventViewWidget = CreateWidget<URSTycoonEventViewWidget>(this, EventViewWidgetClass.Get());
	EventViewWidget->AddToViewport();
	EventViewWidget->SetVisibility(ESlateVisibility::Hidden);

	FoodListWidget = CreateWidget<URSTycoonFoodListWidget>(this, FoodListWidgetClass.Get());
	CameraChangeWidget = CreateWidget<URSTycoonCameraChangeWidget>(this, CameraChangeWidgetClass.Get());
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
		EnhancedInput->BindAction(TileClickAction, ETriggerEvent::Triggered, this, &ARSTycoonPlayerController::OnClickTileOrNPC);
	}

	if (ZoomAction)
	{
		EnhancedInput->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ARSTycoonPlayerController::OnZoom);
		EnhancedInput->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ARSTycoonPlayerController::OnRotateTile);
	}
}

void ARSTycoonPlayerController::SettingInput()
{
	check(IMC)

	auto InputSubsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	InputSubsystem->AddMappingContext(IMC, 0);
}

#pragma endregion

#pragma region Camera
void ARSTycoonPlayerController::SetMainCameraLocationToCenter()
{
	MainCamera->SetLocationToCenter();
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
	MainCamera->SetLocationToCenter();
}

void ARSTycoonPlayerController::OnZoom(const FInputActionValue& Value)
{
	float InputAction = Value.Get<float>();
	if (GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>()->GetState() == ETycoonGameMode::Management)
	{
		if (SelectTileIndex != INDEX_NONE)
		{
			return;
		}

		//탑 카메라
		float OrthoWidth = TopCamera->GetCameraComponent()->OrthoWidth + OrthoWidthSensitivity * InputAction;
		OrthoWidth = FMath::Clamp(OrthoWidth, MinOrthoWidth, MaxOrthoWidth);

		TopCamera->AttachPlayer();
		TopCamera->GetCameraComponent()->SetOrthoWidth(OrthoWidth);
	}
	else
	{
		//메인 카메라
		float CameraLength = MainCamera->GetSpringArmComponent()->TargetArmLength + CameraMoveSensitivity * InputAction;
		CameraLength = FMath::Clamp(CameraLength, MinLengthOfMainCamera, MaxLengthOfMainCamera);

		MainCamera->AttachPlayer();
		MainCamera->GetSpringArmComponent()->TargetArmLength = CameraLength;
	}
}
#pragma endregion

#pragma region TileChange
void ARSTycoonPlayerController::OnRotateTile(const FInputActionValue& Value)
{
	ARSTycoonGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>();
	check(GameMode)

	if (GameMode->GetState() != ETycoonGameMode::Management || SelectTileIndex == INDEX_NONE)
	{
		return;
	}

	ARSTileMap* TileMap = Cast<ARSTileMap>(UGameplayStatics::GetActorOfClass(GetWorld(), ARSTileMap::StaticClass()));
	check(TileMap)

	float Yaw = TileMap->GetTiles()[SelectTileIndex]->GetActorRotation().Yaw;
	const float Sign = Value.Get<float>();
	Yaw += 90 * Sign;

	TileMap->RotateTile(SelectTileIndex, Yaw);
	CheckLimitsOfSale();
}

void ARSTycoonPlayerController::OnClickTileOrNPC()
{
	ARSTycoonGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>();
	check(GameMode)

	if (GameMode->GetState() != ETycoonGameMode::Management)
	{
		return;
	}

	if (NPCInfoWidget->IsInViewport())
	{
		NPCInfoWidget->RemoveFromParent();
	}

	FHitResult HitResult;
	if (GetHitResultUnderCursor(ECC_WorldDynamic, true, HitResult))
	{
		//디버그
		RS_DRAW_DEBUG_SPHERE(GetWorld(), HitResult.Location, 40, 20, FColor::Blue, false, 5, 0, 1.0f);

		AActor* HitActor = HitResult.GetActor();
		if (ARSBaseTile* Tile = Cast<ARSBaseTile>(HitActor))
		{
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

			RS_LOG_F("%s 타일이 선택됬습니다", *HitActor->GetName());
			ManagementWidget->OpenBuyTileLayout();

			EnableSelectTileOutline(Tile->GetActorLocation());
		}
		else if (ARSTycoonNPC* NPC = Cast<ARSTycoonNPC>(HitActor))
		{
			RS_LOG_F("%s NPC가 선택됬습니다", *HitActor->GetName());

			DisableSelectTileOutline();

			NPCInfoWidget->SetNPC(NPC);
			NPCInfoWidget->AddToViewport();

			FVector2D MousePosition;
			GetMousePosition(MousePosition.X, MousePosition.Y);

			NPCInfoWidget->SetPositionInViewport(MousePosition);
		}
		else
		{
			DisableSelectTileOutline();
		}
	}
	//타일을 한번 선택한 후 어딜 누르든 타일 선택이 취소됨
	else
	{
		DisableSelectTileOutline();
	}
}

void ARSTycoonPlayerController::SettingChangeTile()
{
	SelectTileActor = GetWorld()->SpawnActor(SelectTileActorClass);
	SelectTileActor->SetActorHiddenInGame(true);

	CanRotateWidget = CreateWidget(this, CanRotateWidgetClass);
}

void ARSTycoonPlayerController::EnableSelectTileOutline(FVector CenterLocation)
{
	SelectTileActor->SetActorHiddenInGame(false);
	SelectTileActor->SetActorLocation(CenterLocation);

	CanRotateWidget->AddToViewport();

	FVector2D ScreenPos;
	ProjectWorldLocationToScreen(CenterLocation, ScreenPos);
	CanRotateWidget->SetPositionInViewport(ScreenPos);
}

void ARSTycoonPlayerController::DisableSelectTileOutline()
{
	if (SelectTileIndex != INDEX_NONE)
	{
		SelectTileIndex = INDEX_NONE;
		ManagementWidget->CloseBuyTileLayout();
		ManagementWidget->CloseBuyNPCLayout();

		SelectTileActor->SetActorHiddenInGame(true);
		CanRotateWidget->RemoveFromParent();
	}
}
#pragma endregion

void ARSTycoonPlayerController::AddGold(int32 Value)
{
	SetGold(Gold + Value);
	SaleGold += Value;

	if (Value > 0)
	{
		check(AddGoldSound);
		UGameplayStatics::SpawnSoundAtLocation(this, AddGoldSound, FVector::ZeroVector);
	}
	else
	{
		check(SpendGoldSound);
		UGameplayStatics::SpawnSoundAtLocation(this, SpendGoldSound, FVector::ZeroVector);
	}
}

void ARSTycoonPlayerController::AddCustomerCount(int32 Value)
{
	SetCustomerCount(CustomerCount + Value);
}

void ARSTycoonPlayerController::SetGold(int32 Value)
{
	Gold = Value;
	OnChangeGold.Broadcast(Gold);
}

void ARSTycoonPlayerController::SetCustomerCount(int32 Value)
{
	CustomerCount = Value;
	SaleWidget->SetCustomerCount(CustomerCount);
}

void ARSTycoonPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(true);

	SettingInput();
	SettingCamera();
	SettingWidget();
	SettingChangeTile();
}

void ARSTycoonPlayerController::FloatingGold(int32 Amount, FVector WorldLocation)
{
	check(FloatingTextWidget);

	FloatingTextWidget->AddToViewport();

	FVector2D ScreenPos;
	ProjectWorldLocationToScreen(WorldLocation, ScreenPos);
	FloatingTextWidget->SetPositionInViewport(ScreenPos);

	FloatingTextWidget->FloatingValue(Amount);
}
