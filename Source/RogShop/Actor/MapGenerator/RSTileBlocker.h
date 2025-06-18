// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSTileBlocker.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class ROGSHOP_API ARSTileBlocker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARSTileBlocker();


	void RaiseBlocker();  // 타일 진입 시 호출
	void LowerBlocker();  // 몬스터 전멸 시 호출
	FIntPoint GetLinkedTileCoord() const; // 타일코드를 가져올 함수

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FIntPoint LinkedTileCoord;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Collision;

	FVector HiddenLocation;   // 시작 지하 위치
	FVector ShownLocation;    // 올라온 위치

	FRotator HiddenRotation;	//시작 각도
	FRotator ShownRotation;		//올라온 각도

	float AnimationTime = 0.f;
	float TotalTime = 0.5f;

	bool bRaising = false;
	bool bLowering = false;


};
