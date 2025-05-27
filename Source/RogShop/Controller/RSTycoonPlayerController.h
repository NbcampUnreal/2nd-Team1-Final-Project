// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RSTycoonPlayerController.generated.h"

class URSTycoonSaleResultWidget;
class URSTycoonWaitWidget;
class URSTycoonHUDWidget;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class ROGSHOP_API ARSTycoonPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void AddMoney(int32 Value);
	void AddCustomerCount(int32 Value);

	void StartWait();
	void StartSale();
	void EndSale();

	int32 GetMoney() const { return Money;}
	int32 GetCustomerCount() const { return CustomerCount; }
	
protected:
	virtual void BeginPlay() override;
	
public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> InteractAction;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> IMC;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URSTycoonWaitWidget> WaitWidgetType;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URSTycoonHUDWidget> MainHUDType;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URSTycoonSaleResultWidget> SaleResultWidgetType;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ManagementWidgetType;
	
	UPROPERTY()
	TObjectPtr<URSTycoonWaitWidget> WaitWidget;
	
	UPROPERTY()
	TObjectPtr<URSTycoonHUDWidget> MainHUD;
	
	UPROPERTY()
	TObjectPtr<URSTycoonSaleResultWidget> SaleResultWidget;
	
	int32 Money;
	int32 CustomerCount;
};
