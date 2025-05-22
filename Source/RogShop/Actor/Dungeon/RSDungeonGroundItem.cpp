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

	// TODO : �÷��̾��� �κ��丮�� �������� �߰��ϴ� ���� �ۼ�
	//�÷��̾��κ��丮������Ʈ* ������ = Interactor->Get�÷��̾��κ��丮������Ʈ();
	//if (��üũ)
	//{
	//	�÷��̾� �κ��丮 ������Ʈ->�κ��丮�� ������ �߰��ϴ� �Լ� ȣ��;
	// 
	//	Destroy();
	//}
}

