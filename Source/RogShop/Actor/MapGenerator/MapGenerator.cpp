// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGenerator.h"
#include "Engine/World.h"
#include "Containers/Queue.h"

// Sets default values
AMapGenerator::AMapGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AMapGenerator::BeginPlay()
{
	Super::BeginPlay();
	
    RandomStream.Initialize(Seed);

    GenerateMainPath(); 
    ExpandPathToCoverMinTiles(0.5f);
    SpawnTiles();
}
// ��ȿ�� ��ġ���� Ȯ�� (�׸��� �ȿ� �ִ���)
bool AMapGenerator::IsValidPos(FVector2D Pos) const
{
    return Pos.X >= 0 && Pos.X < GridSize && Pos.Y >= 0 && Pos.Y < GridSize;
}

//���� ��ġ���� �̵��� �� �ִ� ���� ��ġ ����
FVector2D AMapGenerator::GetNextDirection(FVector2D Current, TArray<FVector2D>& Visited)
{
    TArray<FVector2D> Candidates;

    // 4���� Ž��
    TArray<FVector2D> Directions = {
        FVector2D(0, 1),   // ��
        FVector2D(0, -1),  // �Ʒ�
        FVector2D(-1, 0),  // ��
        FVector2D(1, 0)    // ��
    };

    for (auto& Dir : Directions)
    {
        FVector2D Next = Current + Dir;
        if (IsValidPos(Next) && !Visited.Contains(Next))
        {
            Candidates.Add(Next);
        }
    }

    // �ĺ��� ������ (-1, -1) ��ȯ
    return Candidates.Num() > 0 ? Candidates[RandomStream.RandRange(0, Candidates.Num() - 1)] : FVector2D(-1, -1);
}

//�������������� ���� ��� ����
void AMapGenerator::GenerateMainPath()
{
    FVector2D Current = FVector2D(0, 0);
    TArray<FVector2D> Path;
    Path.Add(Current);

    const int32 MinPathLength = FMath::Clamp(GridSize + 1, 5, GridSize * GridSize); //�׸��� ����� ���� �������� �ּ� Ÿ�� ���� ����

    while (Path.Num() < 5) //�ּ� 4�� // ���߿� �׸��� ũ�Ⱑ Ŀ���� ���� ���� Ÿ�� ������ ���濹��
    {
        FVector2D Next = GetNextDirection(Current, Path);
        if (Next == FVector2D(-1, -1)) break;

        // �������� ���� ���
        EDir From, To;
        if (Next.X > Current.X) { From = EDir::Right; To = EDir::Left; }
        else if (Next.X < Current.X) { From = EDir::Left; To = EDir::Right; }
        else if (Next.Y > Current.Y) { From = EDir::Up; To = EDir::Down; }
        else { From = EDir::Down; To = EDir::Up; }

        // ����� ���� ����
        TileMap.FindOrAdd(Current).Connections |= From;
        TileMap.FindOrAdd(Next).Connections |= To;
        TileMap[Next].bIsMainPath = true;

        Current = Next;
        Path.Add(Current);
    }

    FindBossRoom(); // ��� �� ���� �� Ÿ���� ���������� ����
}

//������ ��ġ ã�� (BFS�� ���� �� Ÿ�� Ž��)
void AMapGenerator::FindBossRoom()
{
    TQueue<FVector2D> Queue;
    TMap<FVector2D, int32> Distance;

    FVector2D Start = FVector2D(0, 0);
    Queue.Enqueue(Start);
    Distance.Add(Start, 0);

    FVector2D Farthest = Start;
    int32 MaxDist = 0;

    while (!Queue.IsEmpty())
    {
        FVector2D Current;
        Queue.Dequeue(Current);
        int32 CurrDist = Distance[Current];

        // ���� �� �Ÿ� ����
        if (CurrDist > MaxDist)
        {
            MaxDist = CurrDist;
            Farthest = Current;
        }

        // ���� ��ġ�� ���� ���� �������� �̿� Ž��
        EDir Conn = TileMap[Current].Connections;
        TArray<TPair<EDir, FVector2D>> Neighbors = {
            {EDir::Up, Current + FVector2D(0, 1)},
            {EDir::Down, Current + FVector2D(0, -1)},
            {EDir::Left, Current + FVector2D(-1, 0)},
            {EDir::Right, Current + FVector2D(1, 0)}
        };

        for (auto& [Dir, Next] : Neighbors)
        {
            if ((Conn & Dir) != EDir::None && TileMap.Contains(Next) && !Distance.Contains(Next))
            {
                Distance.Add(Next, CurrDist + 1);
                Queue.Enqueue(Next);
            }
        }
    }

    BossRoomPos = Farthest; // ���� �� �� = ������
}

//��ü Ÿ�� ���� �ּ� ���� �̻��� �ǵ��� ��� Ȯ��
void AMapGenerator::ExpandPathToCoverMinTiles(float MinRatio)
{
    const int32 MinTiles = FMath::CeilToInt(GridSize * GridSize * MinRatio);
    TArray<FVector2D> AllUsed;
    TileMap.GenerateKeyArray(AllUsed); // ������� ������ Ÿ�ϵ��� ��ġ ����� �迭�� ������

    // ��ü Ÿ�� ������ �ּ� ���� �̻��� �� ������ �ݺ�
    while (TileMap.Num() < MinTiles)
    {
        FVector2D Base = AllUsed[RandomStream.RandRange(0, AllUsed.Num() - 1)]; // ���� ���� Ÿ�� �� �ϳ��� �������� ����

        TArray<FVector2D> Candidates;

        // �� ������ ���� ����
        TArray<FVector2D> Directions = 
        {
            FVector2D(0, 1), FVector2D(0, -1),
            FVector2D(-1, 0), FVector2D(1, 0)
        };

        Directions.Sort([&](const FVector2D& A, const FVector2D& B) 
        {
            return RandomStream.FRand() < 0.5f;
        }); //Directions �迭 ������ ����

        // Directions �� ���� �������� ���� ��ġ�� �ĺ��� �߰�
        for (const FVector2D& Dir : Directions)
        {
            FVector2D Next = Base + Dir;

            if (IsValidPos(Next) && !TileMap.Contains(Next))
            {
                Candidates.Add(Next);
            }
        }

        // �ĺ��� ������ �н��ϰ� �ٽ� �ݺ�
        if (Candidates.Num() == 0) continue;

        // �ĺ� �� �ϳ� ����
        FVector2D Next = Candidates[0];

        // ���ٸ� ��� ������ ����(50%Ȯ��)
        const bool bDeadEnd = RandomStream.FRand() < 0.5f;

        // ���� ��� (Base �� Next ����)
        EDir From, To;
        if (Next.X > Base.X)
        {
            From = EDir::Right; To = EDir::Left; 
        }
        else if (Next.X < Base.X) 
        { 
            From = EDir::Left; To = EDir::Right; 
        }
        else if (Next.Y > Base.Y)
        { 
            From = EDir::Up; To = EDir::Down;
        }
        else
        { 
            From = EDir::Down; To = EDir::Up; 
        }
        // ���� Ÿ��(Base)�� ���� ���� �߰�
        TileMap.FindOrAdd(Base).Connections |= From;

        // Next�� ���� ���� �߰� (���ٸ����� ��� ����)
        if (!bDeadEnd)
        {
            TileMap.FindOrAdd(Next).Connections |= To;
        }

        // �� Ÿ���� ���� ��ġ�� �߰�
        AllUsed.Add(Next);

    }
}

//Ÿ�� ���� (���� ���⿡ ���� ȸ�� ����)
void AMapGenerator::SpawnTiles()
{
    for (int32 X = 0; X < GridSize; ++X)
    {
        for (int32 Y = 0; Y < GridSize; ++Y)
        {
            FVector2D Pos(X, Y);
            FVector WorldLoc = GetActorLocation() + FVector(X * TileSize, Y * TileSize, 0);

            if (!TileMap.Contains(Pos)) continue;

            FTileData Data = TileMap[Pos];
            FRotator Rot = FRotator::ZeroRotator;
            int ConnBits = (int)Data.Connections;
            int DirCount = FMath::CountBits(ConnBits); // ����� ���� ��
            TSubclassOf<AActor> SelectedTile = nullptr;

            // �������� ��� ���� Ÿ�� ���
            if (BossRoomTile && Pos == BossRoomPos)
            {
                GetWorld()->SpawnActor<AActor>(BossRoomTile, WorldLoc, FRotator::ZeroRotator);
                continue;
            }

            // ���� ���� ���� Ÿ�� �б�
            switch (DirCount)
            {
            case 1: // ���ٸ���
                SelectedTile = DeadEndTile;
                if (ConnBits & (int)EDir::Up)
                    Rot = FRotator(0, 0, 0);     // �Ա� ��
                else if (ConnBits & (int)EDir::Down)
                    Rot = FRotator(0, 180, 0);   // �Ա� �Ʒ�
                else if (ConnBits & (int)EDir::Left)
                    Rot = FRotator(0, 270, 0);   // �Ա� ��
                else if (ConnBits & (int)EDir::Right)
                    Rot = FRotator(0, 90, 0);    // �Ա� ��
                break;

            case 2: // ���� �Ǵ� ������
                if (ConnBits == ((int)(EDir::Up | EDir::Down)))
                {
                    SelectedTile = LineTile;
                    Rot = FRotator(0, 0, 0); // ��
                }
                else if (ConnBits == ((int)(EDir::Left | EDir::Right)))
                {
                    SelectedTile = LineTile;
                    Rot = FRotator(0, 90, 0); // ��
                }
                else
                {
                    SelectedTile = CornerTile;

                    if (ConnBits == ((int)(EDir::Down | EDir::Right)))
                        Rot = FRotator(0, 0, 0);
                    else if (ConnBits == ((int)(EDir::Down | EDir::Left)))
                        Rot = FRotator(0, 90, 0);
                    else if (ConnBits == ((int)(EDir::Up | EDir::Left)))
                        Rot = FRotator(0, 180, 0);
                    else if (ConnBits == ((int)(EDir::Up | EDir::Right)))
                        Rot = FRotator(0, 270, 0);
                }
                break;

            case 3: // T�� Ÿ��
                SelectedTile = TTile;

                if ((ConnBits & (int)EDir::Up) == 0)
                    Rot = FRotator(0, 180, 0); // ��
                else if ((ConnBits & (int)EDir::Down) == 0)
                    Rot = FRotator(0, 0, 0);   // ��
                else if ((ConnBits & (int)EDir::Left) == 0)
                    Rot = FRotator(0, 270, 0); // ��
                else if ((ConnBits & (int)EDir::Right) == 0)
                    Rot = FRotator(0, 90, 0);  // ��
                break;

            case 4: // ���� Ÿ��
                SelectedTile = CrossTile;
                Rot = FRotator(0, 0, 0);
                break;

            default:
                break;
            }

            // ���� ����
            if (SelectedTile)
            {
                GetWorld()->SpawnActor<AActor>(SelectedTile, WorldLoc, Rot);
            }
        }
    }
}
