#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" // 헤더 추가
#include "CookFoodData.generated.h"

class ARSBaseFood;

struct FItemSlot;

UENUM(BlueprintType)
enum class EIngredientType : uint8
{
	Etc,
	Meat,
	Vegetable
};

USTRUCT(BlueprintType)
struct ROGSHOP_API FCookFoodData : public FTableRowBase
{
	GENERATED_BODY()

public:
	bool CanMake(const TArray<FItemSlot>& Ingredients) const;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, int32> NeedIngredients;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Price;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ARSBaseFood> ActorType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Image;
};


// TODO : 텍스처 데이터 추가 및 데이터 테이블 분리
USTRUCT(BlueprintType)
struct ROGSHOP_API FIngredientData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EIngredientType Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Image;
};