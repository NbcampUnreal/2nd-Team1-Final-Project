// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GuideData.h"
#include "RSGuideWidget.generated.h"

class URSGuideButtonWidget;
class UScrollBox;
class UTextBlock;
class UImage;
class UButton;

UCLASS()
class ROGSHOP_API URSGuideWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnGuideButtonClicked(FName CategoryID);

private:
	// 데이터테이블을 이용한 버튼 동적 생성 함수
	void CreateButtons();

	UFUNCTION()
	void OnClickCloseButton();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> GuideScrollBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GuideText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> GuideImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URSGuideButtonWidget> GuideButtonClass;
};
