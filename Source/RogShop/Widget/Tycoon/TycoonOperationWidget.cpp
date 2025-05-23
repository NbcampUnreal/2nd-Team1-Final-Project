// Fill out your copyright notice in the Description page of Project Settings.


#include "TycoonOperationWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "RogShop/UtilDefine.h"
#include "Tycoon/NPC/RSTycoonChefCharacter.h"
#include "Tycoon/NPC/RSTycoonWaiterCharacter.h"

void UTycoonOperationWidget::SetGold(int32 Value)
{
	GoldText->SetText(FText::AsNumber(Value));
}

void UTycoonOperationWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CreateChefButton->OnClicked.AddDynamic(this, &UTycoonOperationWidget::CreateChef);
	CreateWaiterButton->OnClicked.AddDynamic(this, &UTycoonOperationWidget::CreateWaiter);
}

void UTycoonOperationWidget::CreateChef()
{
	FVector2D Center;
	GetWorld()->GetGameViewport()->GetViewportSize(Center);
	Center /= 2;
	
	FHitResult Result;
	GetOwningPlayer()->GetHitResultAtScreenPosition(Center, ECC_Visibility, false, Result);

	if (Result.bBlockingHit)
	{
		RS_LOG("셰프 생성")
		AActor* Chef = GetWorld()->SpawnActor(ChefClass);
		Chef->SetActorLocation(Result.Location + PlusSpawnLo);
	}
}

void UTycoonOperationWidget::CreateWaiter()
{
	FVector2D Center;
	GetWorld()->GetGameViewport()->GetViewportSize(Center);
	Center /= 2;
	
	FHitResult Result;
	GetOwningPlayer()->GetHitResultAtScreenPosition(Center, ECC_Visibility, false, Result);

	if (Result.bBlockingHit)
	{
		RS_LOG("웨이터 생성")
		AActor* Waiter = GetWorld()->SpawnActor(WaiterClass);
		Waiter->SetActorLocation(Result.Location + PlusSpawnLo);
	}
}
