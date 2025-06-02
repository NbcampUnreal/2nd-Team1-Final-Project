// Fill out your copyright notice in the Description page of Project Settings.
#include "RSTycoonBuyNPCWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void URSTycoonBuyNPCWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (NPCButton)
	{
		NPCButton->OnClicked.AddDynamic(this, &URSTycoonBuyNPCWidget::HandleButtonClicked);
	}
}

void URSTycoonBuyNPCWidget::HandleButtonClicked()
{
	OnClick.Broadcast(); // 부모에서 바인딩한 함수 실행
}

void URSTycoonBuyNPCWidget::SetNPCName(const FText& NewText)
{
	if (NPCName)
	{
		NPCName->SetText(NewText);
	}
}