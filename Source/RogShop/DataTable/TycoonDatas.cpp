#include "TycoonDatas.h"
#include "Tycoon/NPC/RSTycoonCustomerCharacter.h"

bool FFoodOrder::operator==(const FFoodOrder& Other) const
{
	return FoodKey == Other.FoodKey && Customer == Other.Customer;
}
