#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" // 헤더 추가
#include "CookFoodData.generated.h"

class ARSBaseFood;

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
	bool CanMake(const TMap<FName, int32>& Ingredients) const; 
	
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
};


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
	TSubclassOf<ARSBaseFood> ActorType;
};