// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RSTycoonPlayerController.generated.h"

class URSTycoonFoodListWidget;
class URSPlayerFloatingDamageWidget;
class URSTycoonEvent;
class URSTycoonEventViewWidget;
class URSTycoonNPCInfoWidget;
struct FInputActionValue;
struct FFoodOrder;
struct FItemSlot;
class URSTycoonInventoryComponent;
class URSIngredientInventoryWidget;
class ARSTycoonCamera;
class URSTycoonManagementWidget;
class URSTycoonSaleWidget;
class URSTycoonSaleResultWidget;
class URSTycoonWaitWidget;
class UInputAction;
class UInputMappingContext;
class URSTycoonCameraChangeWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeGold, int32, Value);

UCLASS()
class ROGSHOP_API ARSTycoonPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	ARSTycoonPlayerController();

#pragma region Mode

public:
	void StartWaitMode(bool bNoAnimation = false);
	void StartSaleMode();
	void EndSaleMode();
	void StartManagementMode();

	UFUNCTION(BlueprintCallable)
	void SetSaleEnable(bool Value);
	UFUNCTION(BlueprintCallable)
	bool CheckLimitsOfSale();

private:
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<URSTycoonEvent>> LimitsForSale;
#pragma endregion

#pragma region Widget

public:
	void AddOrderSlot(FFoodOrder Order);
	void RemoveOrderSlot(FFoodOrder Order);
	void ActiveOrderSlot(FFoodOrder Order, FTimerHandle CookTimer);
	void FinishOrderSlot(FFoodOrder Order);

	void OpenFoodListWidget();

	UFUNCTION(BlueprintCallable)
	void CloseFoodListWidget();

	URSIngredientInventoryWidget* GetInventoryWidget() const { return InventoryWidget; }
	bool IsOpenFoodListUI() const { return bIsOpenFoodListUI; }

private:
	void ChangeMainWidget(UUserWidget* ActiveWidget, bool bNoAnimation = false);
	void SettingWidget();

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URSTycoonWaitWidget> WaitWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URSTycoonSaleWidget> SaleWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URSTycoonSaleResultWidget> SaleResultWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URSTycoonManagementWidget> ManagementWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URSIngredientInventoryWidget> InventoryWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URSTycoonNPCInfoWidget> NPCInfoWidgetClass;

	// 획득 골드 표시 위젯 클래스
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URSPlayerFloatingDamageWidget> FloatingTextWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URSTycoonEventViewWidget> EventViewWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URSTycoonFoodListWidget> FoodListWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URSTycoonCameraChangeWidget> CameraChangeWidgetClass;

	UPROPERTY()
	TObjectPtr<URSTycoonWaitWidget> WaitWidget;

	UPROPERTY()
	TObjectPtr<URSTycoonSaleWidget> SaleWidget;

	UPROPERTY()
	TObjectPtr<URSTycoonSaleResultWidget> SaleResultWidget;

	UPROPERTY()
	TObjectPtr<URSTycoonManagementWidget> ManagementWidget;

	UPROPERTY()
	TObjectPtr<URSIngredientInventoryWidget> InventoryWidget;

	UPROPERTY()
	TObjectPtr<URSTycoonNPCInfoWidget> NPCInfoWidget;

	UPROPERTY()
	TObjectPtr<URSTycoonEventViewWidget> EventViewWidget;

	UPROPERTY()
	TObjectPtr<URSPlayerFloatingDamageWidget> FloatingTextWidget;

	UPROPERTY()
	TObjectPtr<URSTycoonFoodListWidget> FoodListWidget;
	
	UPROPERTY()
	TObjectPtr<URSTycoonCameraChangeWidget> CameraChangeWidget;

	bool bIsOpenFoodListUI;
#pragma endregion

#pragma region Input

protected:
	virtual void SetupInputComponent() override;

private:
	void SettingInput();

public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> InteractAction;

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> TileClickAction;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> ZoomAction;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> IMC;

#pragma endregion

#pragma region Camera
public:
	void SetMainCameraLocationToCenter();
	
private:
	void SettingCamera();

	UFUNCTION()
	void OnZoom(const FInputActionValue& Value);

protected:
	UPROPERTY(EditDefaultsOnly)
	float CameraMoveSensitivity = 4.f;

	UPROPERTY(EditDefaultsOnly)
	float OrthoWidthSensitivity = 20.f;

	UPROPERTY(EditDefaultsOnly)
	float MinLengthOfMainCamera = 50.f;

	UPROPERTY(EditDefaultsOnly)
	float MinOrthoWidth;

private:
	UPROPERTY()
	TObjectPtr<ARSTycoonCamera> MainCamera;

	UPROPERTY()
	TObjectPtr<ARSTycoonCamera> TopCamera;

#pragma endregion

#pragma region TileChange

public:
	int32 GetSelectTileIndex() const { return SelectTileIndex; }

private:
	UFUNCTION()
	void OnRotateTile(const FInputActionValue& Value);
	
	UFUNCTION()
	void OnClickTileOrNPC();
	
	void SettingChangeTile();

	void EnableSelectTileOutline(FVector CenterLocation);
	void DisableSelectTileOutline();


private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> SelectTileActorClass;
	UPROPERTY()
	TObjectPtr<AActor> SelectTileActor;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> CanRotateWidgetClass;
	UPROPERTY()
	TObjectPtr<UUserWidget> CanRotateWidget;

	int32 SelectTileIndex = INDEX_NONE;
#pragma endregion

public:
	UFUNCTION(BlueprintCallable)
	void AddGold(int32 Value);

	void AddCustomerCount(int32 Value);
	void SetGold(int32 Value);
	void SetCustomerCount(int32 Value);

	int32 GetGold() const { return Gold; }
	int32 GetCustomerCount() const { return CustomerCount; }
	URSTycoonInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
	int32 GetSaleGold() const { return SaleGold; }

protected:
	virtual void BeginPlay() override;

public:
	FOnChangeGold OnChangeGold;

	void FloatingGold(int32 Amount, FVector WorldLocation);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<URSTycoonInventoryComponent> InventoryComponent; //인벤토리

private:
	int32 Gold;
	int32 CustomerCount;
	int32 SaleGold; //이번 게임에서 얻은 골드

	// 사운드
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> AddGoldSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> SpendGoldSound;
};
