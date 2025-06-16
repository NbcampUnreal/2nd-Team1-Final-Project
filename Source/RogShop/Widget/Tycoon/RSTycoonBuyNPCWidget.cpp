// Fill out your copyright notice in the Description page of Project Settings.
#include "RSTycoonBuyNPCWidget.h"

#include "RSTycoonPlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/Actor/Tycoon/RSTileMap.h"
#include "Tycoon/NPC/RSTycoonNPC.h"

void URSTycoonBuyNPCWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ARSTycoonNPC* SpawnNPC = SpawnNPCClass.GetDefaultObject();
	NPCNameText->SetText(FText::FromString(SpawnNPC->GetDisplayName()));
	PriceText->SetText(FText::FromString(FString::FromInt(SpawnNPC->GetPrice())));
	NPCImage->SetBrushFromTexture(SpawnNPC->GetThumbnail());
	
	Button->OnClicked.AddDynamic(this, &URSTycoonBuyNPCWidget::OnClickButton);
}

void URSTycoonBuyNPCWidget::OnClickButton()
{
	ARSTycoonPlayerController* Controller = Cast<ARSTycoonPlayerController>(GetOwningPlayer());
	check(Controller)

	if (Controller->GetGold() < Price)
	{
		RS_LOG_C("돈이 부족해 NPC를 구매하지 못 했습니다.", FColor::Red)
		return;
	}
	
	ARSTileMap* TileMap = Cast<ARSTileMap>(UGameplayStatics::GetActorOfClass(GetWorld(), ARSTileMap::StaticClass()));
	check(TileMap)
	TileMap->SpawnActorInMap(SpawnNPCClass);

	Controller->AddGold(-Price);
}
