#include "TycoonDatas.h"

#include "RogShop/UtilDefine.h"
#include "Tycoon/NPC/RSTycoonCustomerCharacter.h"

bool FFoodOrder::Empty()
{
	return FoodKey == FName() || FoodKey == TEXT("") || Customer.Get() == nullptr;
}

bool FFoodOrder::operator==(const FFoodOrder& Other) const
{
	return FoodKey == Other.FoodKey && Customer == Other.Customer;
}
