// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSGuideButtonWidget.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGuideButtonClicked, FName, CategoryID);

UCLASS()
class ROGSHOP_API URSGuideButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	// 카테고리 아이디 세팅 + 버튼 텍스트 세팅용 함수
	UFUNCTION()
	void SetGuideButtonData(FName InGuideID, const FText& InGuideButtonText);

	// 버튼 클릭 이벤트 델리게이트 (부모 위젯에서 바인딩)
	UPROPERTY(BlueprintAssignable)
	FOnGuideButtonClicked OnGuideButtonClicked;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleButtonClicked();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> GuideButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GuideButtonText;

	// 카테고리 아이디 (데이터테이블 RowName 저장)
	UPROPERTY()
	FName GuideID;
};
