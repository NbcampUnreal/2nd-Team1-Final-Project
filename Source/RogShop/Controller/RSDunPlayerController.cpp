// Fill out your copyright notice in the Description page of Project Settings.

#include "RSDunPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "RSCheatManager.h"

ARSDunPlayerController::ARSDunPlayerController()
{
    CheatClass = URSCheatManager::StaticClass();

}

void ARSDunPlayerController::BeginPlay()
{
    Super::BeginPlay();

    AddMapping();

}

void ARSDunPlayerController::AddMapping()
{
    // ���� �߰�
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        // Subsystem�� ���� �츮�� �Ҵ��� IMC�� Ȱ��ȭ
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if (IMC)
            {
                Subsystem->AddMappingContext(IMC, 0);
            }
        }
    }
}

void ARSDunPlayerController::RemoveAllMapping()
{
    // ��� ���� ����
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            Subsystem->ClearAllMappings();
        }
    }
}
