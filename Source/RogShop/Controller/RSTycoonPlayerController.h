// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RSTycoonPlayerController.generated.h"

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

	int32 GetSelectTileIndex() const { return SelectTileIndex; }

private:
	int32 SelectTileIndex = INDEX_NONE;

#pragma endregion

#pragma region Widget

public:
	void AddOrderSlot(const FFoodOrder* Order);
	void RemoveOrderSlot(const FFoodOrder* Order);
	
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

	UFUNCTION()
	void OnTileClick();

public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> InteractAction;

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> TileClickAction;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> IMC;

#pragma endregion

public:
	void AddGold(int32 Value);
	void AddCustomerCount(int32 Value);
	void SetCameraLocationToCenter();

	int32 GetGold() const { return Gold; }
	int32 GetCustomerCount() const { return CustomerCount; }
	URSTycoonInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

protected:
	virtual void BeginPlay() override;

private:
	void SettingCamera();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<URSTycoonInventoryComponent> InventoryComponent; //인벤토리

private:
	UPROPERTY()
	TObjectPtr<ARSTycoonCamera> MainCamera;

	UPROPERTY()
	TObjectPtr<ARSTycoonCamera> TopCamera;

	int32 Gold;
	int32 CustomerCount;
};
