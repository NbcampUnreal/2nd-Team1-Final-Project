// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGenerator.h"
#include "Engine/World.h"
#include "Containers/Queue.h"

// Sets default values
AMapGenerator::AMapGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

// Called when the game starts or when spawned
void AMapGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	GenerateMap(77777);   // 시드에 따른 맵 생성
}

// Called every frame
void AMapGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AMapGenerator::GenerateMap(int32 Seed)
{
    const float TileSize = 4000.f; // 타일 크기 (4000cm)
    const float GridOffset = (GridSize * TileSize) / 2.f; // 맵 중심 기준 좌표

    FRandomStream RandStream(Seed);
    FVector2D Current = StartPos; // 시작 위치: (0, 0)

    VisitedRooms.Empty();
    RoomMap.Empty();
    VisitedRooms.Add(Current); // 시작 방 등록

    // 필요한 최소 방 수 (60%)
    const int32 MinRoomCount = FMath::CeilToInt(GridSize * GridSize * (MinRoomPercent / 100.f));

    // 랜덤워크로 방 생성
    while (VisitedRooms.Num() < MinRoomCount)
    {
        // 인접한 아직 방문하지 않은 방 목록
        TArray<FVector2D> Neighbors = GetValidNeighbors(Current);

        if (Neighbors.Num() > 0)
        {
            // 인접 방 중 무작위 선택하여 이동
            Current = Neighbors[RandStream.RandRange(0, Neighbors.Num() - 1)];
            VisitedRooms.Add(Current);
        }
        else
        {
            // 막혔을 경우: 기존 생성된 방 중 하나에서 다시 시작
            TArray<FVector2D> RoomList = VisitedRooms.Array();
            Current = RoomList[RandStream.RandRange(0, RoomList.Num() - 1)];
        }
    }

    // 방 정보를 RoomMap에 저장
    for (const FVector2D& Pos : VisitedRooms)
    {
        FRoom Room;
        Room.Position = Pos;
        RoomMap.Add(Pos, Room);
    }

    // 시작점에서 가장 먼 방을 보스방으로 지정
    FVector2D BossRoomPos = FindFurthestRoom();
    RoomMap[BossRoomPos].bIsBossRoom = true;

    // 모든 방을 월드에 스폰
    for (const auto& Elem : RoomMap)
    {
        const FVector2D& GridPos = Elem.Value.Position;

        // 격자 좌표를 월드 좌표로 변환
        FVector WorldLocation = FVector(
            (GridPos.X * TileSize) - GridOffset + TileSize / 2,
            (GridPos.Y * TileSize) - GridOffset + TileSize / 2,
            0.f
        );

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;

        // 보스방과 일반 방 각각 블루프린트 액터로 생성
        if (Elem.Value.bIsBossRoom && BossRoomActorClass)
        {
            GetWorld()->SpawnActor<AActor>(BossRoomActorClass, WorldLocation, FRotator::ZeroRotator, SpawnParams);
        }
        else if (NormalRoomActorClass)
        {
            GetWorld()->SpawnActor<AActor>(NormalRoomActorClass, WorldLocation, FRotator::ZeroRotator, SpawnParams);
        }
    }
}
// 현재 위치에서 유효한 인접 방 반환
TArray<FVector2D> AMapGenerator::GetValidNeighbors(FVector2D Current)
{
    TArray<FVector2D> Neighbors;
    TArray<FVector2D> Directions = {
        FVector2D(1, 0), FVector2D(-1, 0),
        FVector2D(0, 1), FVector2D(0, -1)
    };

    for (const FVector2D& Dir : Directions)
    {
        FVector2D Next = Current + Dir;

        // 그리드 범위 내 && 아직 생성되지 않은 위치만 허용
        if (Next.X >= 0 && Next.X < GridSize && Next.Y >= 0 && Next.Y < GridSize && !VisitedRooms.Contains(Next))
        {
            Neighbors.Add(Next);
        }
    }
    return Neighbors;
}
// BFS를 사용해 가장 먼 방을 찾기 (보스방 위치 설정)
FVector2D AMapGenerator::FindFurthestRoom()
{
    TMap<FVector2D, int32> Distances;
    TQueue<FVector2D> Queue;

    Queue.Enqueue(StartPos);
    Distances.Add(StartPos, 0);

    while (!Queue.IsEmpty())
    {
        FVector2D Current;
        Queue.Dequeue(Current);
        int32 CurrDist = Distances[Current];

        for (const FVector2D& Dir : { FVector2D(1,0), FVector2D(-1,0), FVector2D(0,1), FVector2D(0,-1) })
        {
            FVector2D Neighbor = Current + Dir;

            if (!Distances.Contains(Neighbor) && VisitedRooms.Contains(Neighbor))
            {
                Distances.Add(Neighbor, CurrDist + 1);
                Queue.Enqueue(Neighbor);
            }
        }
    }

    FVector2D Furthest = StartPos;
    int32 MaxDist = -1;

    // 가장 먼 거리의 방 찾기
    for (auto& Elem : Distances)
    {
        if (Elem.Value > MaxDist)
        {
            MaxDist = Elem.Value;
            Furthest = Elem.Key;
        }
    }
    return Furthest;
}


