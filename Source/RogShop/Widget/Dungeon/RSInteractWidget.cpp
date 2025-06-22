// Fill out your copyright notice in the Description page of Project Settings.


#include "RSInteractWidget.h"
#include "Components/TextBlock.h"
#include "RSDunPlayerController.h"

void URSInteractWidget::NativeConstruct()
{
    Super::NativeConstruct();

    ARSDunPlayerController* RSDunPlayerController = GetOwningPlayer<ARSDunPlayerController>();
    if (RSDunPlayerController)
    {
        RSDunPlayerController->OnInteractableFound.AddDynamic(this, &URSInteractWidget::UpdateInteractName);
    }

}

void URSInteractWidget::UpdateInteractName(FText NewInteractName)
{
    if (InteractName)
    {
        InteractName->SetText(NewInteractName);
    }
}
