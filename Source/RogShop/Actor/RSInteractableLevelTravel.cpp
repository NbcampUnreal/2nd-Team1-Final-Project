// Fill out your copyright notice in the Description page of Project Settings.


#include "RSInteractableLevelTravel.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARSInteractableLevelTravel::ARSInteractableLevelTravel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(SceneComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	MeshComp->SetupAttachment(SceneComp);
	MeshComp->SetCollisionProfileName("Interactable");
}

// Called when the game starts or when spawned
void ARSInteractableLevelTravel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARSInteractableLevelTravel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARSInteractableLevelTravel::Interact(ARSDunPlayerCharacter* Interactor)
{
	// TODO : ������ �̵��ϱ� ���� ���̺� ó���� �ʿ��ϴ�.

	if (TargetLevelAsset.IsValid())
	{
		// �ε�� ���

		FStringAssetReference AssetRef = TargetLevelAsset.ToSoftObjectPath();
		FString LevelPath = AssetRef.ToString();
		FName LevelName = FName(*FPackageName::GetShortName(LevelPath));
		UGameplayStatics::OpenLevel(GetWorld(), LevelName);
	}
	else if (TargetLevelAsset.IsNull() == false)
	{
		// ���� �ε���� �ʾ��� ���
		
		// ��Ű�� ��� ����: /Game/Maps/MyLevel
		FString LevelPackagePath = TargetLevelAsset.ToSoftObjectPath().GetLongPackageName();

		if (!LevelPackagePath.IsEmpty())
		{
			// ���� �̸��� ����
			FName LevelName = FName(*FPackageName::GetShortName(LevelPackagePath));
			UGameplayStatics::OpenLevel(GetWorld(), LevelName);
		}
	}
}
