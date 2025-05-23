// RSPlayerStatRelicEffect.cpp

#include "RSPlayerStatRelicEffect.h"
#include "RSDunPlayerCharacter.h"
#include "RSDunPlayerController.h"
#include "ShopItemData.h"

void URSPlayerStatRelicEffect::ApplyEffect(ARSDunPlayerCharacter* Player, ARSDunPlayerController* PC, FShopItemData& ItemData)
{
	float FinalValue = 0.0f;

	switch (ItemData.RelicType)
	{
		case ERelicType::MaxHpRelic:
		{
			switch (ItemData.Rarity)
			{
				case ERarity::Common: FinalValue = 10.0f; break;
				case ERarity::Rare: FinalValue = 20.0f; break;
				case ERarity::Epic: FinalValue = 30.0f; break;
				case ERarity::Legendary: FinalValue = 40.0f; break;
			}

			Player->IncreaseMaxHP(FinalValue);

			break;
		}
		//case ERelicType::WalkSpeedRelic: // 종류 추가 예정
		//{
		//	switch (ItemData.Rarity)
		//	{
		//		case ERarity::Common: FinalValue = 10.0f; break;
		//		case ERarity::Rare: FinalValue = 20.0f; break;
		//		case ERarity::Epic: FinalValue = 30.0f; break;
		//		case ERarity::Legendary: FinalValue = 40.0f; break;
		//	}

		//	// Player->IncreaseWalkSpeed(FinalValue);

		//	break;
		//}
		default:
			UE_LOG(LogTemp, Warning, TEXT("Not Define RelicType"));
			break;
	}
}
