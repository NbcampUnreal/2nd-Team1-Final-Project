// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RSMiniMapTileWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROGSHOP_API URSMiniMapTileWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetState(bool bVisited, bool bCurrent, bool bIsBoss); // 타일의 표시 상태 설정 (방문 여부, 현재 위치 여부, 보스방/상점 여부)

	// 타일 좌표 설정 및 반환
	void SetTileCoord(FIntPoint Coord) { TileCoord = Coord; }
	FIntPoint GetTileCoord() const { return TileCoord; }
	
protected:
	// UMG 디자이너에서 연결할 Border (색상을 이걸로 바꿔줌)
	UPROPERTY(meta = (BindWidget))
	class UBorder* TileBorder;

private:
	// 이 위젯이 나타내는 타일의 좌표
	FIntPoint TileCoord;

	// 상태 플래그들
	bool bIsVisited = false;
	bool bIsCurrent = false;
	bool bIsBossRoom = false;

	// 내부적으로 색상 갱신 처리
	void UpdateStyle();
};
