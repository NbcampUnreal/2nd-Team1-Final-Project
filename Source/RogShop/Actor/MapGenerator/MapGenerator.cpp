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
// 유효한 위치인지 확인 (그리드 안에 있는지)
bool AMapGenerator::IsValidPos(FVector2D Pos) const
{
    return Pos.X >= 0 && Pos.X < GridSize && Pos.Y >= 0 && Pos.Y < GridSize;
}

//현재 위치에서 이동할 수 있는 다음 위치 선택
FVector2D AMapGenerator::GetNextDirection(FVector2D Current, TArray<FVector2D>& Visited)
{
    TArray<FVector2D> Candidates;

    // 4방향 탐색
    TArray<FVector2D> Directions = {
        FVector2D(0, 1),   // 위
        FVector2D(0, -1),  // 아래
        FVector2D(-1, 0),  // 왼
        FVector2D(1, 0)    // 오
    };

    for (auto& Dir : Directions)
    {
        FVector2D Next = Current + Dir;
        if (IsValidPos(Next) && !Visited.Contains(Next))
        {
            Candidates.Add(Next);
        }
    }

    // 후보가 없으면 (-1, -1) 반환
    return Candidates.Num() > 0 ? Candidates[RandomStream.RandRange(0, Candidates.Num() - 1)] : FVector2D(-1, -1);
}

//시작점에서부터 메인 경로 생성
void AMapGenerator::GenerateMainPath()
{
    FVector2D Current = FVector2D(0, 0);
    TArray<FVector2D> Path;
    Path.Add(Current);

    const int32 MinPathLength = FMath::Clamp(GridSize + 1, 5, GridSize * GridSize); //그리드 사이즈에 따라 동적으로 최소 타일 개수 생성

    while (Path.Num() < 5) //최소 4개 // 나중에 그리드 크기가 커지면 위의 동적 타일 개수로 변경예정
    {
        FVector2D Next = GetNextDirection(Current, Path);
        if (Next == FVector2D(-1, -1)) break;

        // 현재→다음 방향 계산
        EDir From, To;
        if (Next.X > Current.X) { From = EDir::Right; To = EDir::Left; }
        else if (Next.X < Current.X) { From = EDir::Left; To = EDir::Right; }
        else if (Next.Y > Current.Y) { From = EDir::Up; To = EDir::Down; }
        else { From = EDir::Down; To = EDir::Up; }

        // 양방향 연결 설정
        TileMap.FindOrAdd(Current).Connections |= From;
        TileMap.FindOrAdd(Next).Connections |= To;
        TileMap[Next].bIsMainPath = true;

        Current = Next;
        Path.Add(Current);
    }

    FindBossRoom(); // 경로 중 가장 먼 타일을 보스방으로 설정
}

//보스방 위치 찾기 (BFS로 가장 먼 타일 탐색)
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

        // 가장 먼 거리 갱신
        if (CurrDist > MaxDist)
        {
            MaxDist = CurrDist;
            Farthest = Current;
        }

        // 현재 위치의 연결 정보 기준으로 이웃 탐색
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

    BossRoomPos = Farthest; // 가장 먼 방 = 보스방
}

//전체 타일 수가 최소 비율 이상이 되도록 경로 확장
void AMapGenerator::ExpandPathToCoverMinTiles(float MinRatio)
{
    const int32 MinTiles = FMath::CeilToInt(GridSize * GridSize * MinRatio);
    TArray<FVector2D> AllUsed;
    TileMap.GenerateKeyArray(AllUsed); // 현재까지 생성된 타일들의 위치 목록을 배열로 가져옴

    // 전체 타일 개수가 최소 개수 이상이 될 때까지 반복
    while (TileMap.Num() < MinTiles)
    {
        FVector2D Base = AllUsed[RandomStream.RandRange(0, AllUsed.Num() - 1)]; // 현재 사용된 타일 중 하나를 랜덤으로 선택

        TArray<FVector2D> Candidates;

        // 네 방향을 랜덤 섞기
        TArray<FVector2D> Directions = 
        {
            FVector2D(0, 1), FVector2D(0, -1),
            FVector2D(-1, 0), FVector2D(1, 0)
        };

        Directions.Sort([&](const FVector2D& A, const FVector2D& B) 
        {
            return RandomStream.FRand() < 0.5f;
        }); //Directions 배열 무작위 정렬

        // Directions 중 아직 생성되지 않은 위치만 후보로 추가
        for (const FVector2D& Dir : Directions)
        {
            FVector2D Next = Base + Dir;

            if (IsValidPos(Next) && !TileMap.Contains(Next))
            {
                Candidates.Add(Next);
            }
        }

        // 후보가 없으면 패스하고 다시 반복
        if (Candidates.Num() == 0) continue;

        // 후보 중 하나 선택
        FVector2D Next = Candidates[0];

        // 막다른 길로 만들지 여부(50%확률)
        const bool bDeadEnd = RandomStream.FRand() < 0.5f;

        // 방향 계산 (Base → Next 방향)
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
        // 기존 타일(Base)에 연결 정보 추가
        TileMap.FindOrAdd(Base).Connections |= From;

        // Next에 연결 정보 추가 (막다른길일 경우 제외)
        if (!bDeadEnd)
        {
            TileMap.FindOrAdd(Next).Connections |= To;
        }

        // 새 타일을 사용된 위치로 추가
        AllUsed.Add(Next);

    }
}

//타일 스폰 (연결 방향에 따라 회전 설정)
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
            int DirCount = FMath::CountBits(ConnBits); // 연결된 방향 수
            TSubclassOf<AActor> SelectedTile = nullptr;

            // 보스방일 경우 보스 타일 사용
            if (BossRoomTile && Pos == BossRoomPos)
            {
                GetWorld()->SpawnActor<AActor>(BossRoomTile, WorldLoc, FRotator::ZeroRotator);
                continue;
            }

            // 방향 수에 따라 타일 분기
            switch (DirCount)
            {
            case 1: // 막다른길
                SelectedTile = DeadEndTile;
                if (ConnBits & (int)EDir::Up)
                    Rot = FRotator(0, 0, 0);     // 입구 위
                else if (ConnBits & (int)EDir::Down)
                    Rot = FRotator(0, 180, 0);   // 입구 아래
                else if (ConnBits & (int)EDir::Left)
                    Rot = FRotator(0, 270, 0);   // 입구 왼
                else if (ConnBits & (int)EDir::Right)
                    Rot = FRotator(0, 90, 0);    // 입구 오
                break;

            case 2: // 직선 또는 ㄴ자형
                if (ConnBits == ((int)(EDir::Up | EDir::Down)))
                {
                    SelectedTile = LineTile;
                    Rot = FRotator(0, 0, 0); // ㅣ
                }
                else if (ConnBits == ((int)(EDir::Left | EDir::Right)))
                {
                    SelectedTile = LineTile;
                    Rot = FRotator(0, 90, 0); // ─
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

            case 3: // T형 타일
                SelectedTile = TTile;

                if ((ConnBits & (int)EDir::Up) == 0)
                    Rot = FRotator(0, 180, 0); // ┴
                else if ((ConnBits & (int)EDir::Down) == 0)
                    Rot = FRotator(0, 0, 0);   // ┬
                else if ((ConnBits & (int)EDir::Left) == 0)
                    Rot = FRotator(0, 270, 0); // ┤
                else if ((ConnBits & (int)EDir::Right) == 0)
                    Rot = FRotator(0, 90, 0);  // ├
                break;

            case 4: // 십자 타일
                SelectedTile = CrossTile;
                Rot = FRotator(0, 0, 0);
                break;

            default:
                break;
            }

            // 스폰 실행
            if (SelectedTile)
            {
                GetWorld()->SpawnActor<AActor>(SelectedTile, WorldLoc, Rot);
            }
        }
    }
}
