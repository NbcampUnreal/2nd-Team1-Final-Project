// Fill out your copyright notice in the Description page of Project Settings.


#include "RSDungeonGroundItem.h"
#include "RogShop/UtilDefine.h"
#include "Components/BoxComponent.h"

// Sets default values
ARSDungeonGroundItem::ARSDungeonGroundItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	MeshComp->SetupAttachment(SceneComp);
	MeshComp->SetCollisionProfileName(TEXT("Interactable"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ARSDungeonGroundItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARSDungeonGroundItem::InitItemInfo(FName NewDataTableKey, UStaticMesh* NewMesh)
{
	if (!NewMesh)
	{
		RS_LOG_C("Invalid StaticMesh Used", FColor::Red);
	}

	DataTableKey = NewDataTableKey;

	MeshComp->SetStaticMesh(NewMesh);

	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ARSDungeonGroundItem::Interact(ARSDunPlayerCharacter* Interactor)
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

