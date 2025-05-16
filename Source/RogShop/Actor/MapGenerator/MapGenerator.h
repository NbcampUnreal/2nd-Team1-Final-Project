// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapGenerator.generated.h"

USTRUCT()
struct FRoom
{
	GENERATED_BODY()

	FVector2D Position;       // 2D ���� ��ġ
	bool bIsBossRoom = false; // ������ ����
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
  
    static const int32 GridSize = 3;  // �� ũ�� (3x3)
    static const int32 MinRoomPercent = 60; // �ּ� �� ���� ���� (%) 
    
	FVector2D StartPos = FVector2D(0, 0); // ���� ��ġ: (0,0) = ���� �ϴ�
    TSet<FVector2D> VisitedRooms;  // �湮�� �� ��ġ ���
    TMap<FVector2D, FRoom> RoomMap;  // ���� �� ������ ��
    
	void GenerateMap(int32 Seed); // �� ����
    TArray<FVector2D> GetValidNeighbors(FVector2D Current); // ���� ��ġ���� Ȯ�� ������ ���� �� ��ȯ
    FVector2D FindFurthestRoom(); // ������������ ���� �� �� ã�� (�������)

public:
	UPROPERTY(EditAnywhere, Category = "Room Spawning")
	TSubclassOf<AActor> NormalRoomActorClass;  // �Ϲ� �濡 ����� �������Ʈ ���� Ŭ����

	UPROPERTY(EditAnywhere, Category = "Room Spawning")
	TSubclassOf<AActor> BossRoomActorClass;  // �����濡 ����� �������Ʈ ���� Ŭ����

};
