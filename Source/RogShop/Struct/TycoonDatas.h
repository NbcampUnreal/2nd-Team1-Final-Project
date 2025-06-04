#pragma once

#include "CoreMinimal.h"
#include "TycoonDatas.generated.h"

class ARSTycoonCustomerCharacter;

USTRUCT()
struct ROGSHOP_API FFoodOrder
{
	GENERATED_BODY()
	
public:
	bool Empty();
	
	bool operator==(const FFoodOrder& Other) const;
	
public:
	UPROPERTY()
	TWeakObjectPtr<ARSTycoonCustomerCharacter> Customer;
	
	FName FoodKey;
};
