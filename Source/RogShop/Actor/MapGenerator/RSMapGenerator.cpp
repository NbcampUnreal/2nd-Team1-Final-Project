// Fill out your copyright notice in the Description page of Project Settings.


#include "RSMapGenerator.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Containers/Queue.h"

// Sets default values
ARSMapGenerator::ARSMapGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ARSMapGenerator::BeginPlay()
{
	Super::BeginPlay();
	
    RandomStream.Initialize(Seed);

    GenerateMainPath(); 
    ChooseShopTile();
    ExpandPathToCoverMinTiles(0.5f);
    FindBossRoom(); // ��� �� ���� �� Ÿ���� ���������� ����
    SpawnTiles();
}
// ��ȿ�� ��ġ���� Ȯ�� (�׸��� �ȿ� �ִ���)
bool ARSMapGenerator::IsValidPos(FVector2D Pos) const
{
    return Pos.X >= 0 && Pos.X < GridSize && Pos.Y >= 0 && Pos.Y < GridSize;
}

//���� ��ġ���� �̵��� �� �ִ� ���� ��ġ ����
FVector2D ARSMapGenerator::GetNextDirection(FVector2D Current, TArray<FVector2D>& Visited)
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
void ARSMapGenerator::GenerateMainPath()
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
}

//������ ��ġ ã�� (BFS�� ���� �� Ÿ�� Ž��)
void ARSMapGenerator::FindBossRoom()
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
void ARSMapGenerator::ExpandPathToCoverMinTiles(float MinRatio)
{
    const int32 MinTiles = FMath::CeilToInt(GridSize * GridSize * MinRatio);
    TArray<FVector2D> AllUsed;
    TileMap.GenerateKeyArray(AllUsed);

    int32 DeadEndCount = 0;
    const int32 MaxDeadEnds = 5;
    int32 Attempts = 0;
    const int32 MaxAttempts = 5000;

    while (TileMap.Num() < MinTiles && Attempts++ < MaxAttempts)
    {
        // ���� Ÿ�� �� �ϳ� ����
        TArray<FVector2D> TipCandidates;
        for (const FVector2D& Pos : AllUsed)
        {
            if (GetConnectedNeighborCount(Pos) == 1)
            {
                TipCandidates.Add(Pos);
            }
        }

        if (TipCandidates.Num() == 0) break;

        FVector2D Base = TipCandidates[RandomStream.RandRange(0, TipCandidates.Num() - 1)];

        TArray<FVector2D> Directions = {
            FVector2D(0, 1), FVector2D(0, -1),
            FVector2D(1, 0), FVector2D(-1, 0)
        };

        Directions.Sort([&](const FVector2D&, const FVector2D&) {
            return RandomStream.FRand() < 0.5f;
        });

        for (const FVector2D& Dir : Directions)
        {
            FVector2D Next = Base + Dir;
            if (!IsValidPos(Next) || TileMap.Contains(Next)) continue;

            EDir From, To;
            if (Dir == FVector2D(1, 0)) { From = EDir::Right; To = EDir::Left; }
            else if (Dir == FVector2D(-1, 0)) { From = EDir::Left; To = EDir::Right; }
            else if (Dir == FVector2D(0, 1)) { From = EDir::Up; To = EDir::Down; }
            else { From = EDir::Down; To = EDir::Up; }

            TileMap.FindOrAdd(Base).Connections |= From;
            TileMap.FindOrAdd(Next).Connections |= To;

            for (const FVector2D& Dir2 : { FVector2D(0,1), FVector2D(0,-1), FVector2D(1,0), FVector2D(-1,0) })
            {
                FVector2D Neighbor = Next + Dir2;

                if (!IsValidPos(Neighbor) || !TileMap.Contains(Neighbor))
                    continue;

                // ���� ���
                EDir From2, To2;
                if (Dir2 == FVector2D(1, 0)) { From2 = EDir::Right; To2 = EDir::Left; }
                else if (Dir2 == FVector2D(-1, 0)) { From2 = EDir::Left; To2 = EDir::Right; }
                else if (Dir2 == FVector2D(0, 1)) { From2 = EDir::Up; To2 = EDir::Down; }
                else { From2 = EDir::Down; To2 = EDir::Up; }

                TileMap[Next].Connections |= From2;
                TileMap[Neighbor].Connections |= To2;
            }

            // �ֺ��� �� Ȯ���� �� ������ ���ٸ� ��� ����
            if (GetAvailableNeighborCount(Next) == 0 && DeadEndCount < MaxDeadEnds)
            {
                TileMap[Next].Connections &= ~To;
                ++DeadEndCount;
            }

            AllUsed.Add(Next);
            break;
        }
    }
}
int32 ARSMapGenerator::GetConnectedNeighborCount(FVector2D Pos)
{
    int32 Count = 0;
    for (FVector2D Offset : { FVector2D(0, 1), FVector2D(0, -1), FVector2D(1, 0), FVector2D(-1, 0) })
    {
        if (TileMap.Contains(Pos + Offset)) Count++;
    }
    return Count;
}
int32 ARSMapGenerator::GetAvailableNeighborCount(FVector2D Pos)
{
    int32 Count = 0;
    for (FVector2D Offset : { FVector2D(0, 1), FVector2D(0, -1), FVector2D(1, 0), FVector2D(-1, 0) })
    {
        FVector2D Check = Pos + Offset;
        if (IsValidPos(Check) && !TileMap.Contains(Check)) Count++;
    }
    return Count;
}

//Ÿ�� ���� (���� ���⿡ ���� ȸ�� ����)
void ARSMapGenerator::SpawnTiles()
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

            FString TileName = FString::Printf(TEXT("Tile_%d_%d"), X, Y); //����ũ�� �̸�
            TSoftObjectPtr<UWorld> SelectedLevel = nullptr;

            // �������� ��� ���� Ÿ�� ���
            if (BossRoomTileLevel.IsValid() && Pos == BossRoomPos)
            {
                SelectedLevel = BossRoomTileLevel;
                if (!SelectedLevel.IsValid())
                {
                    SelectedLevel.LoadSynchronous(); //���� �ε�
                }
            }
            else {
                // ���� ���� ���� Ÿ�� �б�
                switch (DirCount)
                {
                case 1: // ���ٸ���
                    SelectedLevel = DeadEndTileLevel;
                    if (!SelectedLevel.IsValid())
                    {
                        SelectedLevel.LoadSynchronous(); //���� �ε�
                    }
                    if (ConnBits & (int)EDir::Up)
                        Rot = FRotator(0, 270, 0);     // �Ա� �Ʒ�
                    else if (ConnBits & (int)EDir::Down)
                        Rot = FRotator(0, 90, 0);   // �Ա� ����
                    else if (ConnBits & (int)EDir::Left)
                        Rot = FRotator(0, 0, 0);   // �Ա� ��
                    else if (ConnBits & (int)EDir::Right)
                        Rot = FRotator(0, 180, 0);    // �Ա� ��
                    break;

                case 2: // ���� �Ǵ� ������
                    if (ConnBits == ((int)(EDir::Up | EDir::Down)))
                    {
                        SelectedLevel = LineTileLevel;
                        if (!SelectedLevel.IsValid())
                        {
                            SelectedLevel.LoadSynchronous(); //���� �ε�
                        }
                        Rot = FRotator(0, 90, 0); // ��
                    }
                    else if (ConnBits == ((int)(EDir::Left | EDir::Right)))
                    {
                        SelectedLevel = LineTileLevel;
                        if (!SelectedLevel.IsValid())
                        {
                            SelectedLevel.LoadSynchronous(); //���� �ε�
                        }
                        Rot = FRotator(0, 0, 0); // ��
                    }
                    else
                    {
                        SelectedLevel = CornerTileLevel;
                        if (!SelectedLevel.IsValid())
                        {
                            SelectedLevel.LoadSynchronous(); //���� �ε�
                        }

                        if (ConnBits == ((int)(EDir::Down | EDir::Right)))
                            Rot = FRotator(0, 180, 0);
                        else if (ConnBits == ((int)(EDir::Down | EDir::Left)))
                            Rot = FRotator(0, 90, 0);
                        else if (ConnBits == ((int)(EDir::Up | EDir::Left)))
                            Rot = FRotator(0, 0, 0);
                        else if (ConnBits == ((int)(EDir::Up | EDir::Right)))
                            Rot = FRotator(0, 270, 0);
                    }
                    break;

                case 3: // T�� Ÿ��
                    SelectedLevel = TTileLevel;
                    if (!SelectedLevel.IsValid())
                    {
                        SelectedLevel.LoadSynchronous(); //���� �ε�
                    }

                    if ((ConnBits & (int)EDir::Up) == 0)
                        Rot = FRotator(0, 90, 0); // ��
                    else if ((ConnBits & (int)EDir::Down) == 0)
                        Rot = FRotator(0, 270, 0);   // ��
                    else if ((ConnBits & (int)EDir::Left) == 0)
                        Rot = FRotator(0, 180, 0); // ��
                    else if ((ConnBits & (int)EDir::Right) == 0)
                        Rot = FRotator(0, 0, 0);  // ��
                    break;

                case 4: // ���� Ÿ��
                    SelectedLevel = CrossTileLevel;
                    if (!SelectedLevel.IsValid())
                    {
                        SelectedLevel.LoadSynchronous(); //���� �ε�
                    }
                    Rot = FRotator(0, 0, 0);
                    break;

                default:
                    break;
                }
            }

            // ���� ����
            UE_LOG(LogTemp, Log, TEXT("Create map"));
            if (SelectedLevel.IsValid())
            {
                UE_LOG(LogTemp, Log, TEXT("MapCreate"));
                StreamTile(SelectedLevel, WorldLoc, Rot, TileName);
                //����NPC����
                if (ShopNPC && Pos == ShopTilePos)
                {
                    FTransform NPCLocation(Rot, WorldLoc + FVector(0, 0, 30));
                    GetWorld()->SpawnActor<AActor>(ShopNPC, NPCLocation);
                }
            }
        }
    }
}


void ARSMapGenerator::StreamTile(TSoftObjectPtr<UWorld> LevelToStream, const FVector & Location, const FRotator & Rotation, const FString & UniqueName)
{
    if (!LevelToStream.IsValid()) return;

    //��ġ�� ȸ���� ���� ��ü
    FTransform LevelTransform;
    LevelTransform.SetLocation(Location);
    LevelTransform.SetRotation(Rotation.Quaternion());


    bool bLoadSuccess = false;

    //�������� ���� ����
    ULevelStreamingDynamic* StreamingLevel = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(
        GetWorld(),
        LevelToStream, 
        LevelTransform, 
        bLoadSuccess,
        UniqueName);

    if (!bLoadSuccess || !StreamingLevel)
    {
        UE_LOG(LogTemp, Warning, TEXT("���� �ε� ����: %s"), *LevelToStream.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("���� �ε� ����: %s"), *LevelToStream.ToString());
    }
}

void ARSMapGenerator::ChooseShopTile() 
{

    TArray<FVector2D> Candidates;

    //�������� ������ Ÿ�Ͽ��� �ĺ� ����
    for (const auto& Pair : TileMap)
    {
        const FVector2D& Pos = Pair.Key;
        if (Pos != BossRoomPos)
        {
            Candidates.Add(Pos);
        }
    }

    // �ĺ����� �ϳ� ����
    if (Candidates.Num() > 0)
    {
        ShopTilePos = Candidates[RandomStream.RandRange(0, Candidates.Num() - 1)];
    }
}
