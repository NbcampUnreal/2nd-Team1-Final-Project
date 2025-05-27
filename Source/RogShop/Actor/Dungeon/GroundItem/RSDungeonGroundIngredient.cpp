// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDungeonGroundIngredient.h"
#include "RogShop/UtilDefine.h"

ARSDungeonGroundIngredient::ARSDungeonGroundIngredient()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ARSDungeonGroundIngredient::InitItemInfo(FName NewDataTableKey, UStaticMesh* NewMesh)
{
	if (!NewMesh)
	{
		RS_LOG_C("Invalid StaticMesh Used", FColor::Red);
	}

	DataTableKey = NewDataTableKey;

	MeshComp->SetStaticMesh(NewMesh);

	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ARSDungeonGroundIngredient::Interact(ARSDunPlayerCharacter* Interactor)
{
	if (!Interactor)
	{
		return;
	}

	// TODO : 플레이어의 인벤토리에 아이템을 추가하는 로직 작성
	//플레이어인벤토리컴포넌트* 변수명 = Interactor->Get플레이어인벤토리컴포넌트();
	//if (널체크)
	//{
	//	플레이어 인벤토리 컴포넌트->인벤토리에 아이템 추가하는 함수 호출;
	// 
	//	Destroy();
	//}
}
