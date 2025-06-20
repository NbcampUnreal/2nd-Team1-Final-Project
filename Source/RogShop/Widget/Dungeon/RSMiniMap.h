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
	void InitializeMap(const TSet<FIntPoint>& TileCoords,const FIntPoint& BossTile);

	// 플레이어 현재 위치 업데이트
	void UpdatePlayerPosition(const FIntPoint& TileCoord);

	UFUNCTION(BlueprintCallable)
	void UpdateTileVisual(const FIntPoint& TileCoord);

protected:
	// 위젯 디자인에서 설정한 CanvasPanel (전체 타일이 이 위에 추가됨)
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* MapCanvas;
	UPROPERTY(meta = (BindWidget))
	UImage* MiniMapBackgroundImage;
	UPROPERTY()
	UImage* PlayerHighlightImage = nullptr;

	// 타일 이미지들
	UPROPERTY(EditAnywhere, Category = "Minimap")
	UTexture2D* StraightTexture;

	UPROPERTY(EditAnywhere, Category = "Minimap")
	UTexture2D* CornerTexture;

	UPROPERTY(EditAnywhere, Category = "Minimap")
	UTexture2D* TTexture;

	UPROPERTY(EditAnywhere, Category = "Minimap")
	UTexture2D* CrossTexture;

	UPROPERTY(EditAnywhere, Category = "Minimap")
	UTexture2D* DeadEndTexture;

	UPROPERTY(EditAnywhere, Category = "Minimap")
	UTexture2D* BossRoomTexture;

	UPROPERTY(EditAnywhere, Category = "Minimap")
	UTexture2D* QuestionTileTexture;

	UPROPERTY(EditDefaultsOnly, Category = "Minimap")
	UTexture2D* HighlightBorderTexture; // 테두리용 텍스처

	TMap<FIntPoint, UImage*> TileImageMap; // 좌표 -> 타일 이미지 매핑

	void SetTileImage(const FIntPoint& Coord, UTexture2D* Texture, const FRotator& Rotation);
};
