#pragma once

#include "CoreMinimal.h"
#include "TycoonDatas.generated.h"

class ARSTycoonCustomerCharacter;

USTRUCT()
struct ROGSHOP_API  FFoodOrder
{
	GENERATED_BODY()
	
public:
	bool operator==(const FFoodOrder& Other) const;
	
public:
	FName FoodKey;
	TWeakObjectPtr<ARSTycoonCustomerCharacter> Customer;
};
