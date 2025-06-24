// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSGameInfoWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;
class URSDataSubsystem;

UCLASS()
class ROGSHOP_API URSGameInfoWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnClickConfirmButton();

	UFUNCTION()
	void HandleLeftButtonClicked();

	UFUNCTION()
	void HandleRightButtonClicked();

private:
	void UpdatePage(int32 PageIndex);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> GameInfoLeftButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> GameInfoRightButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ConfirmButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> GameInfoImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CategoryTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DescriptionTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PageTextBlock;

private:
	int32 CurrentPageIndex = 0; // 현재 페이지
	TArray<FName> PageRowNames; // DataTable의 RowName 목록
	URSDataSubsystem* DataSubsystem;
	
};
