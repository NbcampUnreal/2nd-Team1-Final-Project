// Fill out your copyright notice in the Description page of Project Settings.


#include "RSMiniMap.h"
#include "RSMiniMapTileWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "RSDungeonGameModeBase.h"
#include "RSMapGenerator.h"
#include "Components/CanvasPanelSlot.h"


void URSMiniMap::InitializeMap(const TSet<FIntPoint>& AllTiles, const FIntPoint& BossTile)
{
    for (const FIntPoint& Coord : AllTiles)
    {
        UpdateTileVisual(Coord);
    }
    if (!PlayerHighlightImage && HighlightBorderTexture && MapCanvas)
    {
        PlayerHighlightImage = NewObject<UImage>(this, UImage::StaticClass());
        PlayerHighlightImage->SetBrushFromTexture(HighlightBorderTexture);
        PlayerHighlightImage->SetVisibility(ESlateVisibility::HitTestInvisible);

        MapCanvas->AddChild(PlayerHighlightImage);

        if (UCanvasPanelSlot* MapSlot = Cast<UCanvasPanelSlot>(PlayerHighlightImage->Slot))
        {
            MapSlot->SetSize(FVector2D(300 / 3, 300 / 3));
        }
    }
}

void URSMiniMap::UpdatePlayerPosition(const FIntPoint& TileCoord)
{
    UpdateTileVisual(TileCoord); // 진입한 곳은 항상 시각적으로 확정

    if (!PlayerHighlightImage || !MapCanvas) return;

    if (UCanvasPanelSlot* MapSlot = Cast<UCanvasPanelSlot>(PlayerHighlightImage->Slot))
    {
        FVector2D Pos(TileCoord.X * (300 / 3), TileCoord.Y * (300 / 3));
        MapSlot->SetPosition(Pos);
    }
}

void URSMiniMap::UpdateTileVisual(const FIntPoint& TileCoord)
{
    ARSDungeonGameModeBase* GameMode = Cast<ARSDungeonGameModeBase>(UGameplayStatics::GetGameMode(this));
    if (!GameMode || !MapCanvas) return;

    ARSMapGenerator* MapGen = GameMode->GetMapGenerator();
    if (!MapGen) return;

    const FTileData* TileData = MapGen->GetTileData(TileCoord);

    UTexture2D* TextureToUse = QuestionTileTexture;
    FRotator Rotation = FRotator::ZeroRotator;

    if (TileData && TileData->bVisited)
    {
        Rotation = TileData->Rotation;
        int DirCount = FMath::CountBits((uint8)TileData->Connections);

        switch (DirCount)
        {
        case 1: TextureToUse = DeadEndTexture; break;
        case 2:
            if ((uint8)TileData->Connections == ((uint8)(EDir::Up | EDir::Down)) ||
                (uint8)TileData->Connections == ((uint8)(EDir::Left | EDir::Right)))
                TextureToUse = StraightTexture;
            else
                TextureToUse = CornerTexture;
            break;
        case 3: TextureToUse = TTexture; break;
        case 4: TextureToUse = CrossTexture; break;
        default: break;
        }

        if (TileCoord == MapGen->GetBossTileCoord())
        {
            TextureToUse = BossRoomTexture;
            Rotation = FRotator::ZeroRotator;
        }
    }

    SetTileImage(TileCoord, TextureToUse, Rotation);
}

void URSMiniMap::SetTileImage(const FIntPoint& Coord, UTexture2D* Texture, const FRotator& Rotation)
{
    if (!MapCanvas)
    {
        return;
    }

    if (!MapCanvas || !Texture) return;

    UImage* TileImage = nullptr;

    // 이미지가 이미 생성되어 있는지 확인
    if (TileImageMap.Contains(Coord))
    {
        TileImage = TileImageMap[Coord];
    }
    else
    {
        // 새로 생성
        TileImage = NewObject<UImage>(this, UImage::StaticClass());
        TileImage->SetBrushFromTexture(Texture);
        MapCanvas->AddChild(TileImage);

        // 위치 및 크기 설정
        if (UCanvasPanelSlot* MapSlot = Cast<UCanvasPanelSlot>(TileImage->Slot))
        {
            MapSlot->SetPosition(FVector2D(Coord.X * (300/3), Coord.Y * (300/3)));
            MapSlot->SetSize(FVector2D(300/3, 300/3));
        }

        TileImageMap.Add(Coord, TileImage); // 맵에 저장
    }

    // 텍스처 갱신
    TileImage->SetBrushFromTexture(Texture);

    // 회전 설정
    FWidgetTransform Transform;
    Transform.Angle = Rotation.Yaw;
    TileImage->SetRenderTransform(Transform);
}