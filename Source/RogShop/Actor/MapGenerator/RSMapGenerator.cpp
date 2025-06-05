// Fill out your copyright notice in the Description page of Project Settings.


#include "RSMapGenerator.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "RSDungeonGameModeBase.h"
#include "Engine/World.h"
#include "Containers/Queue.h"


//생성자 : 기본값 초기화
ARSMapGenerator::ARSMapGenerator()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    ShopTilePos = FVector2D::ZeroVector;
    bMapGenerationComplete = false;
    TileSize = 4000.0f;
    Seed = 8888;
    GridSize = 3;
}

// 맵 생성 프로세스 시작
void ARSMapGenerator::BeginPlay()
{
    Super::BeginPlay();

    RandomStream.Initialize(Seed);

    GenerateMainPath();
    ChooseShopTile();
    ExpandPathToCoverMinTiles(0.5f);
    FindBossRoom(); // 경로 중 가장 먼 타일을 보스방으로 설정
    SpawnTiles();
    SpawnBossArenaLevel();
}

//현재 위치에서 이동할 수 있는 다음 위치 선택
FVector2D ARSMapGenerator::GetNextDirection(FVector2D Current, TArray<FVector2D>& Visited)
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
void ARSMapGenerator::GenerateMainPath()
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
        if (Next.X > Current.X)
        {
            From = EDir::Right; To = EDir::Left;
        }
        else if (Next.X < Current.X)
        {
            From = EDir::Left; To = EDir::Right;
        }
        else if (Next.Y > Current.Y)
        {
            From = EDir::Up; To = EDir::Down;
        }
        else
        {
            From = EDir::Down; To = EDir::Up;
        }

        // 양방향 연결 설정
        TileMap.FindOrAdd(Current).Connections |= From;
        TileMap.FindOrAdd(Next).Connections |= To;
        TileMap[Next].bIsMainPath = true;

        Current = Next;
        Path.Add(Current);
    }
}

// 외부에서 시드를 설정할 함수
void ARSMapGenerator::SetSeed(int32 RandomSeed)
{
    Seed = RandomSeed;
}

// 유효한 위치인지 확인 (그리드 안에 있는지)
bool ARSMapGenerator::IsValidPos(FVector2D Pos) const
{
    return Pos.X >= 0 && Pos.X < GridSize && Pos.Y >= 0 && Pos.Y < GridSize;
}
// 연결된 이웃 타일개수 반환
int32 ARSMapGenerator::GetConnectedNeighborCount(FVector2D Pos)
{
    int32 Count = 0;
    for (FVector2D Offset : { FVector2D(0, 1), FVector2D(0, -1), FVector2D(1, 0), FVector2D(-1, 0) })
    {
        if (TileMap.Contains(Pos + Offset))
        {
            Count++;
        }
    }
    return Count;
}
// 주변에서 아직 생성되지 않은 타일 개수 반환
int32 ARSMapGenerator::GetAvailableNeighborCount(FVector2D Pos)
{
    int32 Count = 0;
    for (FVector2D Offset : { FVector2D(0, 1), FVector2D(0, -1), FVector2D(1, 0), FVector2D(-1, 0) })
    {
        FVector2D Check = Pos + Offset;
        if (IsValidPos(Check) && !TileMap.Contains(Check))
        {
            Count++;
        }
    }
    return Count;
}

//보스방 위치 찾기 (BFS로 가장 먼 타일 탐색)
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
    BossWorldLocation = FVector(BossRoomPos.X * TileSize, BossRoomPos.Y * TileSize, 0.f);
}

//전체 타일 수가 최소 비율 이상이 되도록 경로 확장
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
        // 말단 타일 중 하나 선택
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
                {
                    continue;
                }

                // 방향 계산
                EDir From2, To2;
                if (Dir2 == FVector2D(1, 0))
                {
                    From2 = EDir::Right; To2 = EDir::Left;
                }
                else if (Dir2 == FVector2D(-1, 0))
                {
                    From2 = EDir::Left; To2 = EDir::Right;
                }
                else if (Dir2 == FVector2D(0, 1))
                {
                    From2 = EDir::Up; To2 = EDir::Down;
                }
                else
                {
                    From2 = EDir::Down; To2 = EDir::Up;
                }

                TileMap[Next].Connections |= From2;
                TileMap[Neighbor].Connections |= To2;
            }

            // 주변에 더 확장할 곳 없으면 막다른 길로 지정
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

//타일 스폰 (연결 방향에 따라 회전 설정)
void ARSMapGenerator::SpawnTiles()
{
    for (int32 X = 0; X < GridSize; ++X)
    {
        for (int32 Y = 0; Y < GridSize; ++Y)
        {
            FVector2D Pos(X, Y);
            FVector WorldLoc = GetActorLocation() + FVector(X * TileSize, Y * TileSize, 0);

            if (!TileMap.Contains(Pos))
            {
                continue;
            }


            FTileData Data = TileMap[Pos];
            FRotator Rot = FRotator::ZeroRotator;
            int ConnBits = (int)Data.Connections;
            int DirCount = FMath::CountBits(ConnBits); // 연결된 방향 수

            FString TileName = FString::Printf(TEXT("Tile_%d_%d"), X, Y); //유니크한 이름
            TSoftObjectPtr<UWorld> SelectedLevel = nullptr;

            // 보스방일 경우 보스 타일 사용
            if (BossRoomTileLevel.IsValid() && Pos == BossRoomPos)
            {
                SelectedLevel = BossRoomTileLevel;
                if (!SelectedLevel.IsValid())
                {
                    SelectedLevel.LoadSynchronous(); //강제 로드
                }
                if (ConnBits & (int)EDir::Up)
                {
                    Rot = FRotator(0, 270, 0);
                }   // 입구 아래
                else if (ConnBits & (int)EDir::Down)
                {
                    Rot = FRotator(0, 90, 0);
                }  // 입구 오른
                else if (ConnBits & (int)EDir::Left)
                {
                    Rot = FRotator(0, 0, 0);
                }  // 입구 왼
                else if (ConnBits & (int)EDir::Right)
                {
                    Rot = FRotator(0, 180, 0);
                }   // 입구 위
            }
            else {
                // 방향 수에 따라 타일 분기
                switch (DirCount)
                {
                case 1: // 막다른길
                    SelectedLevel = DeadEndTileLevel;
                    if (!SelectedLevel.IsValid())
                    {
                        SelectedLevel.LoadSynchronous(); //강제 로드
                    }
                    if (ConnBits & (int)EDir::Up)
                    {
                        Rot = FRotator(0, 270, 0);
                    }    // 입구 아래
                    else if (ConnBits & (int)EDir::Down)
                    {
                        Rot = FRotator(0, 90, 0);
                    }  // 입구 오른
                    else if (ConnBits & (int)EDir::Left)
                    {
                        Rot = FRotator(0, 0, 0);
                    } // 입구 왼
                    else if (ConnBits & (int)EDir::Right)
                    {
                        Rot = FRotator(0, 180, 0);
                    }  // 입구 위
                    break;

                case 2: // 직선 또는 ㄴ자형
                    if (ConnBits == ((int)(EDir::Up | EDir::Down)))
                    {
                        SelectedLevel = LineTileLevel;
                        if (!SelectedLevel.IsValid())
                        {
                            SelectedLevel.LoadSynchronous(); //강제 로드
                        }
                        Rot = FRotator(0, 90, 0); // ㅣ
                    }
                    else if (ConnBits == ((int)(EDir::Left | EDir::Right)))
                    {
                        SelectedLevel = LineTileLevel;
                        if (!SelectedLevel.IsValid())
                        {
                            SelectedLevel.LoadSynchronous(); //강제 로드
                        }
                        Rot = FRotator(0, 0, 0); // ─
                    }
                    else
                    {
                        SelectedLevel = CornerTileLevel;
                        if (!SelectedLevel.IsValid())
                        {
                            SelectedLevel.LoadSynchronous(); //강제 로드
                        }

                        if (ConnBits == ((int)(EDir::Down | EDir::Right)))
                        {
                            Rot = FRotator(0, 180, 0);
                        }
                        else if (ConnBits == ((int)(EDir::Down | EDir::Left)))
                        {
                            Rot = FRotator(0, 90, 0);
                        }
                        else if (ConnBits == ((int)(EDir::Up | EDir::Left)))
                        {
                            Rot = FRotator(0, 0, 0);
                        }
                        else if (ConnBits == ((int)(EDir::Up | EDir::Right)))
                        {
                            Rot = FRotator(0, 270, 0);
                        }
                    }
                    break;

                case 3: // T형 타일
                    SelectedLevel = TTileLevel;
                    if (!SelectedLevel.IsValid())
                    {
                        SelectedLevel.LoadSynchronous(); //강제 로드
                    }

                    if ((ConnBits & (int)EDir::Up) == 0)
                    {
                        Rot = FRotator(0, 90, 0);
                    } // ┴
                    else if ((ConnBits & (int)EDir::Down) == 0)
                    {
                        Rot = FRotator(0, 270, 0);
                    }  // ┬
                    else if ((ConnBits & (int)EDir::Left) == 0)
                    {
                        Rot = FRotator(0, 180, 0);
                    } // ┤
                    else if ((ConnBits & (int)EDir::Right) == 0)
                    {
                        Rot = FRotator(0, 0, 0);
                    }  // ├
                    break;

                case 4: // 십자 타일
                    SelectedLevel = CrossTileLevel;
                    if (!SelectedLevel.IsValid())
                    {
                        SelectedLevel.LoadSynchronous(); //강제 로드
                    }
                    Rot = FRotator(0, 0, 0);
                    break;

                default:
                    break;
                }
            }

            // 스폰 실행
            if (SelectedLevel.IsValid()) //선택된 레벨이 유효한 레벨인지 확인
            {
                SelectedLevel.LoadSynchronous(); //동기적 로딩
                FTransform TileTransform(Rot, WorldLoc); //타일 위치와 회전을 기반으로 변환
                ULevelStreamingDynamic* StreamingLevel = StreamTile(SelectedLevel, WorldLoc, Rot, TileName); //선택된 타일을 스트리밍 방식으로 로드하고 포인터에 저장
            }
        }
    }
}

ULevelStreamingDynamic* ARSMapGenerator::StreamTile(TSoftObjectPtr<UWorld> LevelToStream, const FVector& Location, const FRotator& Rotation, const FString& UniqueName) //동적으로 UWorld 타일을 로딩 및 배치
{
    if (!LevelToStream.IsValid()) return nullptr;

    //위치와 회전을 담을 객체
    FTransform LevelTransform;
    LevelTransform.SetLocation(Location);
    LevelTransform.SetRotation(Rotation.Quaternion());


    bool bLoadSuccess = false;

    //동적으로 레벨 생성
    ULevelStreamingDynamic* StreamingLevel = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(
        GetWorld(),
        LevelToStream,
        LevelTransform,
        bLoadSuccess,
        UniqueName);
    if (StreamingLevel)
    {
        SpawnedLevels.Add(StreamingLevel);
    }

    if (!bLoadSuccess || !StreamingLevel)
    {
        UE_LOG(LogTemp, Warning, TEXT("레벨 로드 실패: %s"), *LevelToStream.ToString());
    }
    else
    {
        StreamingLevel->OnLevelShown.AddUniqueDynamic(this, &ARSMapGenerator::OnSubLevelLoaded);
        UE_LOG(LogTemp, Log, TEXT("레벨 로드 성공: %s"), *LevelToStream.ToString());
    }
    return StreamingLevel;
}

// 보스 아레나 레벨 스폰 (맵 외곽)
FVector ARSMapGenerator::SpawnBossArenaLevel()
{
    if (!BossArenaLevel.IsValid())
    {
        BossArenaLevel.LoadSynchronous();
    }

    FVector ArenaOffset = FVector(TileSize * GridSize * 2, TileSize * GridSize * 2, 0.0f); // 현재 맵과 떨어진 좌표
    FString ArenaUniqueName = TEXT("BossArena");

    bool bSuccess = false;
    FTransform ArenaTransform;
    ArenaTransform.SetLocation(ArenaOffset);
    ArenaTransform.SetRotation(FQuat::Identity);

    ULevelStreamingDynamic* StreamingLevel = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(
        GetWorld(),
        BossArenaLevel,
        ArenaTransform,
        bSuccess,
        ArenaUniqueName
    );

    return ArenaOffset;
}

// 상점 타일 위치 선택 (보스방을 제외한 타일 중에서 무작위로 선택)
void ARSMapGenerator::ChooseShopTile()
{

    TArray<FVector2D> Candidates;

    //보스방을 제외한 타일에서 후보 선출
    for (const auto& Pair : TileMap)
    {
        const FVector2D& Pos = Pair.Key;
        if (Pos != BossRoomPos)
        {
            Candidates.Add(Pos);
        }
    }

    // 후보에서 하나 선택
    if (Candidates.Num() > 0)
    {
        ShopTilePos = Candidates[RandomStream.RandRange(0, Candidates.Num() - 1)];
    }
}


// 스트리밍 타일 로딩 완료 시 호출
void ARSMapGenerator::OnSubLevelLoaded()
{
    UE_LOG(LogTemp, Warning, TEXT("서브 레벨 로딩 완료됨"));
    CheckAllTilesLoaded();

    //지연을 두고 로드 체크
    FTimerHandle TempHandle;
    GetWorld()->GetTimerManager().SetTimer(TempHandle, this, &ARSMapGenerator::CheckAllTilesLoaded, 0.2f, false);
}

// 전체 타일이 로드되었는지 확인
void ARSMapGenerator::CheckAllTilesLoaded()
{
    bool bAllLoaded = true;
    for (ULevelStreamingDynamic* Level : SpawnedLevels)
    {
        if (!Level || !Level->IsLevelLoaded() || !Level->IsLevelVisible())
        {
            bAllLoaded = false;
            break;
        }
    }

    if (bAllLoaded && !bIsMapLoaded)
    {
        bIsMapLoaded = true;

        //GameMode에 직접 알림
        if (AGameModeBase* GM = UGameplayStatics::GetGameMode(this))
        {
            if (ARSDungeonGameModeBase* MyGM = Cast<ARSDungeonGameModeBase>(GM))
            {
                MyGM->NotifyMapReady(); // 델리게이트 호출
            }
        }

        UE_LOG(LogTemp, Warning, TEXT("모든 타일 로딩 완료. GameMode로 전달"));
    }
    else if (!bAllLoaded)
    {
        // 재시도 타이머
        FTimerHandle RetryHandle;
        GetWorld()->GetTimerManager().SetTimer(RetryHandle, this, &ARSMapGenerator::CheckAllTilesLoaded, 0.3f, false);
    }
}

// 로딩 완료 상태 반환
bool ARSMapGenerator::IsMapFullyLoaded() const
{
    return bIsMapLoaded;
}

















