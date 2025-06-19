// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonNPCInfoWidget.h"

#include "RSTycoonPlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Tycoon/NPC/RSTycoonNPC.h"

void URSTycoonNPCInfoWidget::SetNPC(ARSTycoonNPC* NPC)
{
	NameText->SetText(FText::FromString(NPC->GetDisplayName()));

	InfoNPC = NPC;
}

void URSTycoonNPCInfoWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SellButton->OnClicked.AddDynamic(this, &URSTycoonNPCInfoWidget::OnClickSellButton);
}

void URSTycoonNPCInfoWidget::OnClickSellButton()
{
	if (InfoNPC.IsValid())
	{
		ARSTycoonPlayerController* PlayerController = GetWorld()->GetFirstPlayerController<ARSTycoonPlayerController>();
		PlayerController->AddGold(InfoNPC->GetPrice());
		
		//[추가], 돈 추가/감소 이펙트 (이펙트 안에 있는 숫자가 Value로 변경)
		PlayerController->FloatingGold(InfoNPC->GetPrice(), InfoNPC->GetActorLocation());

		InfoNPC->Destroy();
		InfoNPC = nullptr;

		RemoveFromParent();
	}
}
