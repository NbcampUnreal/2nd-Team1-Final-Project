// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSMiniMap.generated.h"

/**
 * 
 */
class URSMiniMapTileWidget;
class UCanvasPanel;
class UImage;

UCLASS()
class ROGSHOP_API URSMiniMap : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// 맵 전체 초기화 (던전의 타일 구성 정보 제공)
	void InitializeMap(const TSet<FIntPoint>& TileCoords, FIntPoint BossTile);

	// 플레이어 현재 위치 업데이트
	void UpdatePlayerPosition(FIntPoint PlayerCoord);

protected:
	// 위젯 디자인에서 설정한 CanvasPanel (전체 타일이 이 위에 추가됨)
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* MapCanvas;
	UPROPERTY(meta = (BindWidget))
	UImage* MiniMapBackgroundImage;

	// 타일 위젯 클래스
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URSMiniMapTileWidget> TileWidgetClass;

private:
	// 좌표별 타일 위젯들을 저장
	TMap<FIntPoint, URSMiniMapTileWidget*> TileWidgets;

	// 방문한 타일 좌표들
	TSet<FIntPoint> VisitedTiles;

	// 보스, 상점 방의 좌표
	FIntPoint BossRoom;
};
