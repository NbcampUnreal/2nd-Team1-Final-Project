// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RSDunMainWidget.h"
#include "RSDunPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class ROGSHOP_API ARSDunPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ARSDunPlayerController();

	virtual void BeginPlay() override;

public:
	void AddMapping();

	void RemoveAllMapping();

	void ShowRSDunMainWidget();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> DodgeAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> InteractionAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> NormalAttackAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> StrongAttackAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> FirstWeaponSlotAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> SecondWeaponSlotAction;

	UPROPERTY()
	URSDunMainWidget* RSDunMainWidget;

protected:
	// 블루프린트에서 지정할 수 있도록 TSubclassOf로 선언
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> RSDunMainWidgetClass;
};
