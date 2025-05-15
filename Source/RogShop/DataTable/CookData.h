#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" // 헤더 추가
#include "CookData.generated.h"

USTRUCT(BlueprintType)
struct ROGSHOP_API FCookData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cook")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cook")
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cook")
	FSoftObjectPath Mesh;
};



USTRUCT(BlueprintType)
struct ROGSHOP_API FIngredientData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ingredient")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ingredient")
	FString Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ingredient")
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ingredient")
	FSoftObjectPath Mesh;
};