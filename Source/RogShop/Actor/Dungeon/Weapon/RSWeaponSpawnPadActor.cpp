// Fill out your copyright notice in the Description page of Project Settings.
#include "RSWeaponSpawnPadActor.h"
#include "DungeonItemData.h"
#include "RSInteractableWeapon.h"

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
    TArray<FDungeonItemData*> AllWeapons;
    WeaponDataTable->GetAllRows(TEXT("WeaponSpawn"), AllWeapons);

    TArray<FDungeonItemData*> FilteredWeapons;

    for (FDungeonItemData* Item : AllWeapons)
    {
        if (Item && Item->ItemRarity == EItemRarity::Common && Item->ItemType == EItemType::Weapon)
        {
            FilteredWeapons.Add(Item);
        }
    }

    // 선택된 무기 저장용 배열
    TArray<FDungeonItemData*> SelectedWeapons;

    // 중복 방지를 위해 랜덤 선택 후 제거
    while (SelectedWeapons.Num() < NumberOfWeapons && FilteredWeapons.Num() > 0)
    {
        int32 Index = FMath::RandRange(0, FilteredWeapons.Num() - 1);
        SelectedWeapons.Add(FilteredWeapons[Index]);
        FilteredWeapons.RemoveAt(Index); // 이 줄이 "중복 제거" 핵심!
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

        ARSInteractableWeapon* InteractableWeapon = GetWorld()->SpawnActor<ARSInteractableWeapon>(
            ARSInteractableWeapon::StaticClass(),
            SpawnLoc,
            FRotator::ZeroRotator
        );

        UStaticMesh* ItemStaticMesh = SelectedWeapons[i]->ItemStaticMesh;
        TSubclassOf<ARSDungeonItemBase> ItemClass = SelectedWeapons[i]->ItemClass;
        if (ItemStaticMesh && ItemClass)
        {
            InteractableWeapon->InitInteractableWeapon(ItemStaticMesh, ItemClass);
        }

        // UE_LOG(LogTemp, Warning, TEXT("SelectedWeapon ID: %s"), *SelectedWeapons[i]->ItemID.ToString());
    }
}

// Called every frame
void ARSWeaponSpawnPadActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

