// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSBaseTile.generated.h"

#define GENERATED_TILE() \
	static FName GetStaticTileKey() { return StaticClass()->GetFName(); } \
	virtual FName GetTileKey() const override { return GetStaticTileKey(); }


UCLASS()
class ROGSHOP_API ARSBaseTile : public AActor
{
	GENERATED_BODY()

public:
	ARSBaseTile();

	//GENERATED_TILE을 쓰기 위한 함수들
	static FName GetStaticTileKey() { return StaticClass()->GetFName(); }
	virtual FName GetTileKey() const { return GetStaticTileKey(); }
	
	virtual void Tick(float DeltaTime) override;
	virtual void ResetAll();
	virtual void Interact(ACharacter* InteractCharacter);

	FVector GetTileSize();
	FString GetTileDisplayName() const { return TileDisplayName; }
	int32 GetPrice() const { return Price; }
	TObjectPtr<UTexture2D> GetThumbnail() const { return Thumbnail; }
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> SceneComp;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> Plane;
	
	UPROPERTY(EditDefaultsOnly)
	FString TileDisplayName = TEXT("기본");
	
	//데이터 테이블로 옮길수도?
	UPROPERTY(EditDefaultsOnly)
	int32 Price = 10;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> Thumbnail;
	
};
 