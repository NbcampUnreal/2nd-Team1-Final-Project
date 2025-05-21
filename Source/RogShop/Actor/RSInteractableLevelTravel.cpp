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
	// TODO : 레벨을 이동하기 전에 세이브 처리가 필요하다.

	if (TargetLevelAsset.IsValid())
	{
		// 로드된 경우

		FStringAssetReference AssetRef = TargetLevelAsset.ToSoftObjectPath();
		FString LevelPath = AssetRef.ToString();
		FName LevelName = FName(*FPackageName::GetShortName(LevelPath));
		UGameplayStatics::OpenLevel(GetWorld(), LevelName);
	}
	else if (TargetLevelAsset.IsNull() == false)
	{
		// 아직 로드되지 않았을 경우
		
		// 패키지 경로 예시: /Game/Maps/MyLevel
		FString LevelPackagePath = TargetLevelAsset.ToSoftObjectPath().GetLongPackageName();

		if (!LevelPackagePath.IsEmpty())
		{
			// 레벨 이름만 추출
			FName LevelName = FName(*FPackageName::GetShortName(LevelPackagePath));
			UGameplayStatics::OpenLevel(GetWorld(), LevelName);
		}
	}
}
