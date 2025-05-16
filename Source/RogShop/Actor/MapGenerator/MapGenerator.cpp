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
	
	GenerateMap(77777);   // �õ忡 ���� �� ����
}

// Called every frame
void AMapGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AMapGenerator::GenerateMap(int32 Seed)
{
    const float TileSize = 4000.f; // Ÿ�� ũ�� (4000cm)
    const float GridOffset = (GridSize * TileSize) / 2.f; // �� �߽� ���� ��ǥ

    FRandomStream RandStream(Seed);
    FVector2D Current = StartPos; // ���� ��ġ: (0, 0)

    VisitedRooms.Empty();
    RoomMap.Empty();
    VisitedRooms.Add(Current); // ���� �� ���

    // �ʿ��� �ּ� �� �� (60%)
    const int32 MinRoomCount = FMath::CeilToInt(GridSize * GridSize * (MinRoomPercent / 100.f));

    // ������ũ�� �� ����
    while (VisitedRooms.Num() < MinRoomCount)
    {
        // ������ ���� �湮���� ���� �� ���
        TArray<FVector2D> Neighbors = GetValidNeighbors(Current);

        if (Neighbors.Num() > 0)
        {
            // ���� �� �� ������ �����Ͽ� �̵�
            Current = Neighbors[RandStream.RandRange(0, Neighbors.Num() - 1)];
            VisitedRooms.Add(Current);
        }
        else
        {
            // ������ ���: ���� ������ �� �� �ϳ����� �ٽ� ����
            TArray<FVector2D> RoomList = VisitedRooms.Array();
            Current = RoomList[RandStream.RandRange(0, RoomList.Num() - 1)];
        }
    }

    // �� ������ RoomMap�� ����
    for (const FVector2D& Pos : VisitedRooms)
    {
        FRoom Room;
        Room.Position = Pos;
        RoomMap.Add(Pos, Room);
    }

    // ���������� ���� �� ���� ���������� ����
    FVector2D BossRoomPos = FindFurthestRoom();
    RoomMap[BossRoomPos].bIsBossRoom = true;

    // ��� ���� ���忡 ����
    for (const auto& Elem : RoomMap)
    {
        const FVector2D& GridPos = Elem.Value.Position;

        // ���� ��ǥ�� ���� ��ǥ�� ��ȯ
        FVector WorldLocation = FVector(
            (GridPos.X * TileSize) - GridOffset + TileSize / 2,
            (GridPos.Y * TileSize) - GridOffset + TileSize / 2,
            0.f
        );

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;

        // ������� �Ϲ� �� ���� �������Ʈ ���ͷ� ����
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
// ���� ��ġ���� ��ȿ�� ���� �� ��ȯ
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

        // �׸��� ���� �� && ���� �������� ���� ��ġ�� ���
        if (Next.X >= 0 && Next.X < GridSize && Next.Y >= 0 && Next.Y < GridSize && !VisitedRooms.Contains(Next))
        {
            Neighbors.Add(Next);
        }
    }
    return Neighbors;
}
// BFS�� ����� ���� �� ���� ã�� (������ ��ġ ����)
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

    // ���� �� �Ÿ��� �� ã��
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


