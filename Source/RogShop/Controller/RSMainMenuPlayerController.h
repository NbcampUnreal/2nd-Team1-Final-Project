// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RSMainMenuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API ARSMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class UUserWidget> MainMenuWidgetClass;

private:
	UPROPERTY()
	class UUserWidget* MainMenuWidget;
};
