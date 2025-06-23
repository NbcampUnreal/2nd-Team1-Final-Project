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

UCLASS()
class ROGSHOP_API URSGuideWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

	// 버튼 클릭 시 호출되는 함수
	UFUNCTION()
	void OnGuideButtonClicked(FName CategoryID);

private:
	// 데이터테이블에서 버튼 리스트 동적 생성 함수
	void CreateButtons();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> GuideScrollBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GuideText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> GuideImage;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URSGuideButtonWidget> GuideButtonClass;
};
