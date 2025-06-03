// Fill out your copyright notice in the Description page of Project Settings.
#include "RSWeaponSpawnPadActor.h"
#include "RSDataSubsystem.h"
#include "ItemInfoData.h"
#include "RSDungeonGroundWeapon.h"

// Sets default values
ARSWeaponSpawnPadActor::ARSWeaponSpawnPadActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PadMesh"));
	RootComponent = PadMesh;
}

// Called when the game starts or when spawned
void ARSWeaponSpawnPadActor::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapons();
}

void ARSWeaponSpawnPadActor::SpawnWeapons()
{
    if (!WeaponDataTable) return;

    // 데이터 테이블에서 모든 Row 가져오기
    TArray<FItemInfoData*> AllWeapons;
    WeaponDataTable->GetAllRows(TEXT("WeaponSpawn"), AllWeapons);
    
    // 데이터 테이블에서 모든 Row에 대한 Row Name을 가져오기
    TArray<FName> AllRowNames = WeaponDataTable->GetRowNames();

    TArray<FItemInfoData*> FilteredWeapons;
    TArray<FName> FilteredRowNames;
    
    for (int i = 0; i < AllWeapons.Num(); ++i)
    {
        if (AllWeapons[i] && AllWeapons[i]->ItemRarity == EItemRarity::Common && AllWeapons[i]->ItemType == EItemType::Weapon)
        {
            FilteredWeapons.Add(AllWeapons[i]);
            FilteredRowNames.Add(AllRowNames[i]);
        }
    }

    // 선택된 무기 저장용 배열
    TArray<FItemInfoData*> SelectedWeapons;
    TArray<FName> SelectedRowNames;

    // 중복 방지를 위해 랜덤 선택 후 제거
    while (SelectedWeapons.Num() < NumberOfWeapons && FilteredWeapons.Num() > 0)
    {
        int32 Index = FMath::RandRange(0, FilteredWeapons.Num() - 1);
        SelectedWeapons.Add(FilteredWeapons[Index]);
        SelectedRowNames.Add(FilteredRowNames[Index]);
        FilteredWeapons.RemoveAt(Index); // 이 줄이 "중복 제거" 핵심!
        FilteredRowNames.RemoveAt(Index); // 이 줄이 "중복 제거" 핵심!
    }

    FVector Origin;
    FVector BoxExtent;
    GetActorBounds(true, Origin, BoxExtent);

    // 발판의 맨 위, 정중앙 위치
    FVector BaseLocation = Origin + FVector(0, 0, BoxExtent.Z + 50);

    // 발판의 Y 길이
    float PlatformLengthY = BoxExtent.Y * 2;

    // 무기 개수
    int32 WeaponCount = SelectedWeapons.Num();
    if (WeaponCount <= 1) return; // 1개면 간격 나눌 필요 없음

    // 무기 간 간격 계산 (발판 길이 내에서 균등하게)
    float ActualSpacing = PlatformLengthY / (WeaponCount - 1);

    // 시작 위치 (왼쪽 끝부터 시작)
    FVector StartLocation = BaseLocation - FVector(0, BoxExtent.Y, 0);

    // 선택한 무기들 스폰
    for (int32 i = 0; i < SelectedWeapons.Num(); ++i)
    {
        FVector SpawnLoc = StartLocation + FVector(0, i * ActualSpacing, 0);

        ARSDungeonGroundWeapon* GroundWeapon = GetWorld()->SpawnActor<ARSDungeonGroundWeapon>(
            ARSDungeonGroundWeapon::StaticClass(),
            SpawnLoc,
            FRotator::ZeroRotator
        );

        FName CurDataTableKey = SelectedRowNames[i];
        UStaticMesh* ItemStaticMesh = SelectedWeapons[i]->ItemStaticMesh;

        FDungeonWeaponData* Data = GetWorld()->GetGameInstance()->GetSubsystem<URSDataSubsystem>()->WeaponClass->FindRow<FDungeonWeaponData>(CurDataTableKey, TEXT("Get WeaponData"));
        
        TSubclassOf<ARSDungeonItemBase> WeaponClass = Data->WeaponClass;
        if (GroundWeapon && ItemStaticMesh && WeaponClass)
        {
            GroundWeapon->InitInteractableWeapon(CurDataTableKey, ItemStaticMesh, WeaponClass);
        }

        // UE_LOG(LogTemp, Warning, TEXT("SelectedWeapon ID: %s"), *SelectedWeapons[i]->ItemID.ToString());
    }
}

// Called every frame
void ARSWeaponSpawnPadActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

