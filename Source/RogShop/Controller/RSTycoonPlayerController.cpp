// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonPlayerController.h"

#include "CanvasItem.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "RSIngredientInventoryWidget.h"
#include "RSTycoonGameModeBase.h"
#include "RSTycoonInventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/HUD.h"
#include "GameFramework/SpringArmComponent.h"
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
	
	CustomerCount = 0;

	SaleWidget->SetGold(Gold);
	SaleWidget->SetCustomerCount(0);
}

void ARSTycoonPlayerController::EndSaleMode()
{
	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
	ChangeMainWidget(SaleResultWidget);

	SaleWidget->RemoveAllOrderSlots();
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
		EnhancedInput->BindAction(TileClickAction, ETriggerEvent::Triggered, this, &ARSTycoonPlayerController::OnClickTile);
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
void ARSTycoonPlayerController::SetCameraLocationToCenter()
{
	MainCamera->SetLocationToCenter();
	TopCamera->SetLocationToCenter();
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
	SetCameraLocationToCenter();

	//SetViewTarget으로 뷰를 설정하면 이론상 다음 프레임에 행렬이 변경되어야 하는데 실제로 NextTick에 실행해보면
	//갱신되지 않아 이상한데에 찍혀 물리적인 딜레이를 주었다.
	// FTimerHandle TimerHandle;
	// GetWorldTimerManager().SetTimer(TimerHandle, [&]()
	// {
	// 	SetMaxLengthOfMainCamera();
	// }, 0.05f, false);
}

void ARSTycoonPlayerController::SetMaxLengthOfMainCamera()
{
	/*
	ARSTileMap* TileMap = Cast<ARSTileMap>(UGameplayStatics::GetActorOfClass(GetWorld(), ARSTileMap::StaticClass()));
	check(TileMap)

	FVector Center = TileMap->GetMapCenter();
	FVector HalfSize = TileMap->GetMapSize() * 0.5f;

	TArray<FVector> Vertices;
	Vertices.SetNum(4);
	Vertices[0] = FVector(Center.X + HalfSize.X, Center.Y - HalfSize.Y, Center.Z);
	Vertices[1] = FVector(Center.X + HalfSize.X, Center.Y + HalfSize.Y, Center.Z);
	Vertices[2] = FVector(Center.X - HalfSize.X, Center.Y - HalfSize.Y, Center.Z);
	Vertices[3] = FVector(Center.X - HalfSize.X, Center.Y + HalfSize.Y, Center.Z);

	//반복문 이전 세팅
	bool bAllVertexInScreen;
	MainCamera->GetSpringArmComponent()->TargetArmLength = 0;
	MainCamera->GetSpringArmComponent()->MarkRenderTransformDirty();
	MainCamera->GetSpringArmComponent()->UpdateComponentToWorld();

	int32 ScreenW, ScreenH;
	GetViewportSize(ScreenW, ScreenH);
	RS_LOG_F("Screen : %d %d", ScreenW, ScreenH)

	do
	{
		MainCamera->GetSpringArmComponent()->TargetArmLength += CameraMoveSensitivity;
		MainCamera->GetSpringArmComponent()->MarkRenderTransformDirty();
		MainCamera->GetSpringArmComponent()->UpdateComponentToWorld();

		UpdateCameraManager(0.0f);
		
		bAllVertexInScreen = true;
		FVector2D ScreenPosition;
		for (FVector Vertex : Vertices)
		{
			ProjectWorldLocationToScreen(Vertex, ScreenPosition);

			//디버그용
			RS_LOG_F_C("%f %f", FColor::Orange, ScreenPosition.X, ScreenPosition.Y);
			FVector WorldLo, WorldFront;
			DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y, WorldLo, WorldFront);
			DrawDebugSphere(GetWorld(), WorldLo, 2, 30, FColor::Orange, false, 10);
			
			if (ScreenPosition.X < 0 || ScreenPosition.X > ScreenW ||
				ScreenPosition.Y < 0 || ScreenPosition.Y > ScreenH)
			{
				bAllVertexInScreen = false;
			}
		}
	} while (!bAllVertexInScreen);

	MaxCameraLengthOfMainCamera = MainCamera->GetSpringArmComponent()->TargetArmLength;
	*/
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

		//최댓값 설정에 너무 많은 시간을 뺏김. 제거
		// if (OrthoWidth >= MaxTopCameraOrthoWidth)
		// {
		// 	OrthoWidth = MaxTopCameraOrthoWidth;
		// 	TopCamera->SetLocationToCenter();
		// }
		// else
		// {
		//		TopCamera->AttachPlayer();
		// }

		if (OrthoWidth < MinOrthoWidth)
		{
			OrthoWidth = MinOrthoWidth;
		}

		TopCamera->AttachPlayer();
		TopCamera->GetCameraComponent()->SetOrthoWidth(OrthoWidth);
	}
	else
	{
		//메인 카메라
		float CameraLength = MainCamera->GetSpringArmComponent()->TargetArmLength + CameraMoveSensitivity * InputAction;

		//최댓값 설정에 너무 많은 시간을 뺏김. 제거
		// if (PerspectiveFov >= MaxCameraLengthOfMainCamera)
		// {
		// 	PerspectiveFov = MaxCameraLengthOfMainCamera;
		// 	MainCamera->SetLocationToCenter();
		// }
		// else
		// {
		// 	MainCamera->AttachPlayer();
		// }

		if (CameraLength < MinLengthOfMainCamera)
		{
			CameraLength = MinLengthOfMainCamera;
		}

		MainCamera->AttachPlayer();
		MainCamera->GetSpringArmComponent()->TargetArmLength = CameraLength;
	}
}
#pragma endregion

#pragma region TileChange
void ARSTycoonPlayerController::OnClickTile()
{
	ARSTycoonGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>();
	check(GameMode)

	if (GameMode->GetState() != ETycoonGameMode::Management)
	{
		return;
	}

	FHitResult HitResult;
	if (GetHitResultUnderCursor(ECC_Visibility, true, HitResult))
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

			SelectTileActor->SetActorHiddenInGame(false);
			SelectTileActor->SetActorLocation(Tile->GetActorLocation());
		}
	}
	//타일을 한번 선택한 후 어딜 누르든 타일 선택이 취소됨
	else if (SelectTileIndex != INDEX_NONE)
	{
		SelectTileIndex = INDEX_NONE;
		ManagementWidget->CloseBuyTileLayout();
		ManagementWidget->CloseBuyNPCLayout();
		
		SelectTileActor->SetActorHiddenInGame(true);
	}
}

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
}

void ARSTycoonPlayerController::SettingChangeTile()
{
	SelectTileActor = GetWorld()->SpawnActor(SelectTileActorClass);
	SelectTileActor->SetActorHiddenInGame(true);
}
#pragma endregion

void ARSTycoonPlayerController::AddGold(int32 Value)
{
	SetGold(Gold + Value);
}

void ARSTycoonPlayerController::AddCustomerCount(int32 Value)
{
	SetCustomerCount(CustomerCount + Value);
}

void ARSTycoonPlayerController::SetGold(int32 Value)
{
	Gold = Value;
	SaleWidget->SetGold(Gold);
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
