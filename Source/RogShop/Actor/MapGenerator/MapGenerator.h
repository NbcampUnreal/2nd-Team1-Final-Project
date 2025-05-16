// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapGenerator.generated.h"

USTRUCT()
struct FRoom
{
	GENERATED_BODY()

	FVector2D Position;       // 2D 격자 위치
	bool bIsBossRoom = false; // 보스방 여부
};


UCLASS()
class ROGSHOP_API AMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
  
    static const int32 GridSize = 3;  // 맵 크기 (3x3)
    static const int32 MinRoomPercent = 60; // 최소 방 생성 비율 (%) 
    
	FVector2D StartPos = FVector2D(0, 0); // 시작 위치: (0,0) = 왼쪽 하단
    TSet<FVector2D> VisitedRooms;  // 방문한 방 위치 목록
    TMap<FVector2D, FRoom> RoomMap;  // 실제 방 데이터 맵
    
	void GenerateMap(int32 Seed); // 맵 생성
    TArray<FVector2D> GetValidNeighbors(FVector2D Current); // 현재 위치에서 확장 가능한 인접 방 반환
    FVector2D FindFurthestRoom(); // 시작지점에서 가장 먼 방 찾기 (보스방용)

public:
	UPROPERTY(EditAnywhere, Category = "Room Spawning")
	TSubclassOf<AActor> NormalRoomActorClass;  // 일반 방에 사용할 블루프린트 액터 클래스

	UPROPERTY(EditAnywhere, Category = "Room Spawning")
	TSubclassOf<AActor> BossRoomActorClass;  // 보스방에 사용할 블루프린트 액터 클래스

};
