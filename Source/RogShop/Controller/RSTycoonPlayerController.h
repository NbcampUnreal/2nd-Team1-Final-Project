// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RSTycoonPlayerController.generated.h"

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
/**
 * 
 */
UCLASS()
class ROGSHOP_API ARSTycoonPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	ARSTycoonPlayerController();

#pragma region Mode

public:
	void StartWaitMode();
	void StartSaleMode();
	void EndSaleMode();
	void StartManagementMode();

#pragma endregion

#pragma region Widget

public:
	void AddOrderSlot(FFoodOrder Order);
	void RemoveOrderSlot(FFoodOrder Order);
	void ActiveOrderSlot(FFoodOrder Order, FTimerHandle CookTimer);
	void FinishOrderSlot(FFoodOrder Order);
	
	URSIngredientInventoryWidget* GetInventoryWidget() const { return InventoryWidget; }

private:
	void ChangeMainWidget(UUserWidget* ActiveWidget);
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
	void SetCameraLocationToCenter();
	
private:
	void SettingCamera();

	UFUNCTION(BlueprintCallable)
	void SetMaxLengthOfMainCamera();
	
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
	void OnClickTile();
	UFUNCTION()
	void OnRotateTile(const FInputActionValue& Value);
	
private:
	int32 SelectTileIndex = INDEX_NONE;
#pragma endregion 
	
public:
	void AddGold(int32 Value);
	void AddCustomerCount(int32 Value);

	int32 GetGold() const { return Gold; }
	int32 GetCustomerCount() const { return CustomerCount; }
	URSTycoonInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<URSTycoonInventoryComponent> InventoryComponent; //인벤토리
	
private:
	int32 Gold;
	int32 CustomerCount;
};
