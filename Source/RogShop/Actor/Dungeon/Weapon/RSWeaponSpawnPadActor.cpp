// Fill out your copyright notice in the Description page of Project Settings.
#include "RSWeaponSpawnPadActor.h"
#include "RSDataSubsystem.h"
#include "ItemInfoData.h"
#include "RSDungeonGroundWeapon.h"
#include "RSBaseAreaGameModeBase.h"

#include "Kismet/GameplayStatics.h"

ARSWeaponSpawnPadActor::ARSWeaponSpawnPadActor()
{
	PrimaryActorTick.bCanEverTick = false;

    SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(SceneComp);

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    MeshComp->SetupAttachment(SceneComp);
}

void ARSWeaponSpawnPadActor::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapons();
}

void ARSWeaponSpawnPadActor::SpawnWeapons()
{
    URSDataSubsystem* DataSubsystem = GetGameInstance()->GetSubsystem<URSDataSubsystem>();

    if (!DataSubsystem || !DataSubsystem->WeaponInfo || !DataSubsystem->WeaponDetail)
    {
        return;
    }

    // 데이터 테이블에서 모든 무기 Row 가져오기
    TArray<FItemInfoData*> AllWeapons;
    DataSubsystem->WeaponInfo->GetAllRows(TEXT("WeaponSpawn"), AllWeapons);
    
    // 데이터 테이블에서 모든 무기 Row에 대한 Row Name을 가져오기
    TArray<FName> AllRowNames = DataSubsystem->WeaponInfo->GetRowNames();

    TArray<FName> FilteredRowNames;
    
    // 등급이 Common인 무기의 키만 저장
    for (int i = 0; i < AllWeapons.Num(); ++i)
    {
        if (AllWeapons[i] && AllWeapons[i]->ItemRarity == EItemRarity::Common)
        {
            FilteredRowNames.Add(AllRowNames[i]);
        }
    }

    FName RandomRowName;

    // 게임 모드에서 이미 스폰된 무기 키 목록을 가져오기 위해서 사용
    ARSBaseAreaGameModeBase* GameMode = Cast<ARSBaseAreaGameModeBase>(UGameplayStatics::GetGameMode(this));

    if (!GameMode)
    {
        return;
    }

    // 이미 생성된 RowName은 제외한 후보 목록 만들기
    TArray<FName> AvailableRowNames;
    for (const FName& RowName : FilteredRowNames)
    {
        if (!GameMode->GetSpawnedWeaponRowNames().Contains(RowName))
        {
            AvailableRowNames.Add(RowName);
        }
    }

    // 후보 목록에서 랜덤 선택
    if (AvailableRowNames.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, AvailableRowNames.Num() - 1);
        RandomRowName = AvailableRowNames[RandomIndex];
    }
    else
    {
        return;
    }

    // 선택된 무기 키 기록
    GameMode->AddSpawnedWeaponRowName(RandomRowName);

    // 선택한 키에 해당하는 무기 스폰
    FVector Origin;
    FVector BoxExtent;
    GetActorBounds(true, Origin, BoxExtent);
    FVector SpawnLocation = Origin + FVector(0, 0, BoxExtent.Z + 50);

    FItemInfoData* WeaponData = DataSubsystem->WeaponInfo->FindRow<FItemInfoData>(RandomRowName, TEXT("SpawnWeapon"));
    FDungeonWeaponData* WeaponClassData = DataSubsystem->WeaponDetail->FindRow<FDungeonWeaponData>(RandomRowName, TEXT("SpawnWeapon"));

    if (WeaponData && WeaponClassData && WeaponData->ItemStaticMesh && WeaponClassData->WeaponClass)
    {
        ARSDungeonGroundWeapon* GroundWeapon = GetWorld()->SpawnActor<ARSDungeonGroundWeapon>(
            ARSDungeonGroundWeapon::StaticClass(),
            SpawnLocation,
            FRotator::ZeroRotator
        );

        if (GroundWeapon)
        {
            GroundWeapon->InitInteractableWeapon(RandomRowName, WeaponData->ItemStaticMesh, WeaponClassData->WeaponClass);
        }
    }
}
