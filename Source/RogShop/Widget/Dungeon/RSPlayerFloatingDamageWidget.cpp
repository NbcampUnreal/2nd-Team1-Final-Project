#include "RSPlayerFloatingDamageWidget.h"
#include "Components/TextBlock.h"
#include "RSDunPlayerController.h"

void URSPlayerFloatingDamageWidget::NativeOnInitialized()
{
    ARSDunPlayerController* RSDunPlayerController = GetOwningPlayer<ARSDunPlayerController>();

    if (RSDunPlayerController)
    {
        RSDunPlayerController->OnFloatDamage.AddDynamic(this, &URSPlayerFloatingDamageWidget::ShowDamage);
    }
}

void URSPlayerFloatingDamageWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 텍스트는 기본 숨김 상태로 세팅
    if (TextDamage)
    {
        TextDamage->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void URSPlayerFloatingDamageWidget::ShowDamage(float Damage)
{
    if (!TextDamage)
    {
        return;
    }

    TextDamage->SetText(FText::AsNumber(FMath::RoundToInt(Damage * -1)));
    TextDamage->SetVisibility(ESlateVisibility::Visible);

    // 페이드 아웃 애니메이션 재생 (기존 애니 재생 중이면 재시작)
    if (FadeAnim)
    {
        PlayAnimation(FadeAnim);
    }

    // 1초 후에 숨기기
    GetWorld()->GetTimerManager().ClearTimer(HideTextTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(
        HideTextTimerHandle,
        this,
        &URSPlayerFloatingDamageWidget::OnFadeOutFinished,
        1.0f,
        false
    );
}

void URSPlayerFloatingDamageWidget::OnFadeOutFinished()
{
    if (TextDamage)
    {
        TextDamage->SetVisibility(ESlateVisibility::Collapsed);
        TextDamage->SetText(FText::GetEmpty());
    }
}
