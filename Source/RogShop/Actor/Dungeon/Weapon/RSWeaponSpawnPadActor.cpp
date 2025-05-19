// Fill out your copyright notice in the Description page of Project Settings.
#include "RSWeaponSpawnPadActor.h"
#include "ShopItemData.h"

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

    // ������ ���̺��� ��� Row ��������
    TArray<FShopItemData*> AllWeapons;
    WeaponDataTable->GetAllRows(TEXT("WeaponSpawn"), AllWeapons);

    TArray<FShopItemData*> FilteredWeapons;

    for (FShopItemData* Item : AllWeapons)
    {
        if (Item && Item->Rarity == ERarity::Common && Item->ItemList == EItemList::Weapon)
        {
            FilteredWeapons.Add(Item);
        }
    }

    // ���õ� ���� ����� �迭
    TArray<FShopItemData*> SelectedWeapons;

    // �ߺ� ������ ���� ���� ���� �� ����
    while (SelectedWeapons.Num() < NumberOfWeapons && FilteredWeapons.Num() > 0)
    {
        int32 Index = FMath::RandRange(0, FilteredWeapons.Num() - 1);
        SelectedWeapons.Add(FilteredWeapons[Index]);
        FilteredWeapons.RemoveAt(Index); // �� ���� "�ߺ� ����" �ٽ�!
    }

    FVector Origin;
    FVector BoxExtent;
    GetActorBounds(true, Origin, BoxExtent);

    // ������ �� ��, ���߾� ��ġ
    FVector BaseLocation = Origin + FVector(0, 0, BoxExtent.Z + 50);

    // ������ Y ����
    float PlatformLengthY = BoxExtent.Y * 2;

    // ���� ����
    int32 WeaponCount = SelectedWeapons.Num();
    if (WeaponCount <= 1) return; // 1���� ���� ���� �ʿ� ����

    // ���� �� ���� ��� (���� ���� ������ �յ��ϰ�)
    float ActualSpacing = PlatformLengthY / (WeaponCount - 1);

    // ���� ��ġ (���� ������ ����)
    FVector StartLocation = BaseLocation - FVector(0, BoxExtent.Y, 0);

    // ������ ����� ����
    for (int32 i = 0; i < SelectedWeapons.Num(); ++i)
    {
        FVector SpawnLoc = StartLocation + FVector(0, i * ActualSpacing, 0);
        GetWorld()->SpawnActor<AActor>(
            SelectedWeapons[i]->WeaponClass,
            SpawnLoc,
            FRotator::ZeroRotator
        );

        // UE_LOG(LogTemp, Warning, TEXT("SelectedWeapon ID: %s"), *SelectedWeapons[i]->ItemID.ToString());
    }
}

// Called every frame
void ARSWeaponSpawnPadActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

