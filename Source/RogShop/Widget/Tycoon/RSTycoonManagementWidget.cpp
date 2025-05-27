// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonManagementWidget.h"

#include "RSTycoonGameModeBase.h"
#include "RSTycoonPlayerController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void URSTycoonManagementWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ARSTycoonGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ARSTycoonGameModeBase>();
	// check(GameMode)

	ARSTycoonPlayerController* Controller = GetWorld()->GetFirstPlayerController<ARSTycoonPlayerController>();
	check(Controller)

	GoldText->SetText(FText::FromString(FString::FromInt(Controller->GetGold())));
	
	ExpandTileButton->OnClicked.AddDynamic(this, &URSTycoonManagementWidget::OnClickExpandTile);
}

void URSTycoonManagementWidget::OnClickExpandTile()
{
	
}
