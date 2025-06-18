// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSDunMainHUDWidget.generated.h"

class URSPlayerStatusWidget;
class URSInGameMenuWidget;
class URSPlayerInventoryWidget;
class URSInteractWidget;
class URSBossHPBarWidget;

UCLASS()
class ROGSHOP_API URSDunMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;

public:
    void HideAllWidgets();

    void HandleInGameMenuWidget();
    void HandlePlayerInventoryWidget();

    void ShowInteractWidget();
    void HideInteractWidget();

    URSBossHPBarWidget* GetBossHPBarWidget() const;

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (BindWidget, AllowPrivateAccess = "true"))
    TObjectPtr<URSPlayerStatusWidget> PlayerStatusWidget;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (BindWidget, AllowPrivateAccess = "true"))
    TObjectPtr<URSInGameMenuWidget> InGameMenuWidget;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (BindWidget, AllowPrivateAccess = "true"))
    TObjectPtr<URSPlayerInventoryWidget> PlayerInventoryWidget;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (BindWidget, AllowPrivateAccess = "true"))
    TObjectPtr<URSInteractWidget> InteractWidget;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (BindWidget, AllowPrivateAccess = "true"))
    TObjectPtr<URSBossHPBarWidget> WBP_BossInfoWidget;
};
