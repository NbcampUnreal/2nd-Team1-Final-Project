// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RSTycoonPlayerController.generated.h"

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

private:
	void ChangeMainWidget(UUserWidget* ActiveWidget);
	void SettingWidget();

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URSTycoonWaitWidget> WaitWidgetType;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URSTycoonSaleWidget> SaleWidgetType;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URSTycoonSaleResultWidget> SaleResultWidgetType;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URSTycoonManagementWidget> ManagementWidgetType;

	UPROPERTY()
	TObjectPtr<URSTycoonWaitWidget> WaitWidget;

	UPROPERTY()
	TObjectPtr<URSTycoonSaleWidget> SaleWidget;

	UPROPERTY()
	TObjectPtr<URSTycoonSaleResultWidget> SaleResultWidget;

	UPROPERTY()
	TObjectPtr<URSTycoonManagementWidget> ManagementWidget;
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

protected:
	virtual void BeginPlay() override;

private:
	void SettingCamera();

private:
	UPROPERTY()
	TObjectPtr<ARSTycoonCamera> MainCamera;

	UPROPERTY()
	TObjectPtr<ARSTycoonCamera> TopCamera;

	int32 Gold;
	int32 CustomerCount;
};
