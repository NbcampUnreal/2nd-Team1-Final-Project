// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSDunMainHUDWidget.generated.h"

class URSPlayerStatusWidget;
class URSInGameMenuWidget;

UCLASS()
class ROGSHOP_API URSDunMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;

public:
    void HandleInGameMenuWidget();

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (BindWidget, AllowPrivateAccess = "true"))
    TObjectPtr<URSPlayerStatusWidget> PlayerStatusWidget;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (BindWidget, AllowPrivateAccess = "true"))
    TObjectPtr<URSInGameMenuWidget> InGameMenuWidget;
};
