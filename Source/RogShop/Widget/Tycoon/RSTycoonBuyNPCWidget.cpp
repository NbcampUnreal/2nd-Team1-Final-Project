// Fill out your copyright notice in the Description page of Project Settings.
#include "RSTycoonBuyNPCWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "RogShop/Actor/Tycoon/RSTileMap.h"
#include "Tycoon/NPC/RSTycoonNPC.h"

void URSTycoonBuyNPCWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ARSTycoonNPC* SpawnNPC = SpawnNPCClass.GetDefaultObject();
	NPCNameText->SetText(FText::FromString(SpawnNPC->GetDisplayName()));
	PriceText->SetText(FText::FromString(FString::FromInt(SpawnNPC->GetPrice())));
	
	Button->OnClicked.AddDynamic(this, &URSTycoonBuyNPCWidget::OnClickButton);
}

void URSTycoonBuyNPCWidget::OnClickButton()
{
	ARSTileMap* TileMap = Cast<ARSTileMap>(UGameplayStatics::GetActorOfClass(GetWorld(), ARSTileMap::StaticClass()));
	check(TileMap)

	TileMap->SpawnActorInMap(SpawnNPCClass);
}
