#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MonsterAttackTraceData.generated.h"

USTRUCT(BlueprintType)
struct FMonsterAttackTraceData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackTrace")
	FVector TraceBoxHalfSize;	// 트레이스 반경 (옆으로 얼마만큼 쏠지)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackTrace")
	float TraceLength;			// 트레이스 길이 (앞으로 얼마만큼 쏠지)

	UPROPERTY(EditAnywhere, Category = "AttackTrace")
	float TraceForwardOffset;	// 소켓 시작점 앞뒤 보정

	UPROPERTY(EditAnywhere, Category = "AttackTrace")
	float TraceRightOffset;     // 소켓 시작점 좌우 보정

	UPROPERTY(EditAnywhere, Category = "AttackTrace")
	float TraceUpOffset;        // 소켓 시작점 높이 보정

	UPROPERTY(EditAnywhere, Category = "AttackTrace")
	FName SocketLocation;		// 트레이스가 쏴질 소켓의 시작점

};
