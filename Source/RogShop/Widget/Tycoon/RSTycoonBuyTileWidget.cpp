// Fill out your copyright notice in the Description page of Project Settings.


#include "RSTycoonBuyTileWidget.h"

#include "RSTycoonPlayerController.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "RogShop/UtilDefine.h"
#include "RogShop/Actor/Tycoon/RSTileMap.h"
#include "RogShop/Actor/Tycoon/Tile/RSBaseTile.h"

void URSTycoonBuyTileWidget::SetInfo(ARSBaseTile* Tile)
{
	TileKey = Tile->GetTileKey();
	Price = Tile->GetPrice();

	TileImage->SetBrushFromTexture(Tile->GetThumbnail());
	NameText->SetText(FText::FromString(Tile->GetTileDisplayName()));
	PriceText->SetText(FText::FromString(FString::FromInt(Price)));
}

void URSTycoonBuyTileWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button->OnClicked.AddDynamic(this, &URSTycoonBuyTileWidget::OnClickButton);
}

void URSTycoonBuyTileWidget::OnClickButton()
{
	ARSTycoonPlayerController* Controller = Cast<ARSTycoonPlayerController>(GetOwningPlayer());
	check(Controller)

	if (Controller->GetGold() < Price)
	{
		RS_LOG_C("돈이 부족해 Tile을 구매하지 못 했습니다", FColor::Red)
		return;
	}

	ARSTileMap* TileMap = Cast<ARSTileMap>(UGameplayStatics::GetActorOfClass(GetWorld(), ARSTileMap::StaticClass()));
	check(TileMap)

	int32 SelectTileIndex = Controller->GetSelectTileIndex();
	int32 PrePrice = TileMap->GetTiles()[SelectTileIndex]->GetPrice();
	
	Controller->AddGold(PrePrice - Price);
	TileMap->ChangeTile(SelectTileIndex, TileKey);

	Controller->CheckLimitsOfSale();
}
