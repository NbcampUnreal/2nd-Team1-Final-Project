// Fill out your copyright notice in the Description page of Project Settings.


#include "RSMiniMap.h"
#include "RSMiniMapTileWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"

// 던전 생성 시 타일 전체를 미니맵에 배치
void URSMiniMap::InitializeMap(const TSet<FIntPoint>& TileCoords, FIntPoint BossTile)
{
	if (!TileWidgetClass || !MapCanvas || !MiniMapBackgroundImage)
	{
		return; // 필수 값 누락 체크
	}

	UCanvasPanelSlot* BgSlot = Cast<UCanvasPanelSlot>(MiniMapBackgroundImage->Slot);
	if (!BgSlot) return;

	FVector2D BackgroundSize = BgSlot->GetSize();
	const int32 GridSize = 3;

	float TileSize = 400/3;


	FIntPoint CenterOffset(0, 2);

	BossRoom = BossTile;

	for (FIntPoint Coord : TileCoords)
	{
		// 각 타일 위젯 생성
		URSMiniMapTileWidget* Tile = CreateWidget<URSMiniMapTileWidget>(this, TileWidgetClass);
		Tile->SetTileCoord(Coord);

		// CanvasPanel 위에 위치 설정
		UCanvasPanelSlot* TileSlot = MapCanvas->AddChildToCanvas(Tile);

		FIntPoint LocalCoord = Coord - CenterOffset;

		FVector2D TilePos = MiniMapBackgroundImage->RenderTransform.Translation
			+ FVector2D(LocalCoord.X * TileSize, -LocalCoord.Y * TileSize);

		TileSlot->SetSize(FVector2D(TileSize, TileSize));
		TileSlot->SetPosition(TilePos);
		TileSlot->SetZOrder(10);

		// 초기 상태: 미방문, 현재 위치 아님, 보스방/상점 여부
		Tile->SetState(false, false, Coord == BossTile);

		// 맵에 등록
		TileWidgets.Add(Coord, Tile);
	}

}

// 플레이어가 이동할 때마다 호출해서 상태 갱신
void URSMiniMap::UpdatePlayerPosition(FIntPoint PlayerCoord)
{
	for (auto& Elem : TileWidgets)
	{
		FIntPoint Coord = Elem.Key;

		bool bVisited = VisitedTiles.Contains(Coord) || Coord == PlayerCoord;
		bool bCurrent = Coord == PlayerCoord;
		bool bBoss = (Coord == BossRoom);

		Elem.Value->SetState(bVisited, bCurrent, bBoss);
	}

	// 방문 기록에 추가
	VisitedTiles.Add(PlayerCoord);
}