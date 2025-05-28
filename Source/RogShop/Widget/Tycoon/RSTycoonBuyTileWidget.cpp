// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonBuyTileWidget.h"

#include "RSTycoonPlayerController.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "RogShop/Actor/Tycoon/RSTileMap.h"
#include "RogShop/Actor/Tycoon/Tile/RSBaseTile.h"

void URSTycoonBuyTileWidget::SetInfo(ARSBaseTile* Tile)
{
	TileKey = Tile->GetTileKey();

	TileImage->SetBrushFromTexture(Tile->GetThumbnail());
	NameText->SetText(FText::FromString(Tile->GetTileDisplayName()));
	PriceText->SetText(FText::FromString(FString::FromInt(Tile->GetPrice())));
}

void URSTycoonBuyTileWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Button->OnClicked.AddDynamic(this, &URSTycoonBuyTileWidget::OnClickButton);
}

void URSTycoonBuyTileWidget::OnClickButton()
{
	ARSTycoonPlayerController* Controller = Cast<ARSTycoonPlayerController>(GetOwningPlayer());
	check(Controller)

	ARSTileMap* TileMap = Cast<ARSTileMap>(UGameplayStatics::GetActorOfClass(GetWorld(), ARSTileMap::StaticClass()));
	check(TileMap)

	int32 SelectTileIndex = Controller->GetSelectTileIndex();
	TileMap->ChangeTile(SelectTileIndex, TileKey);
}
