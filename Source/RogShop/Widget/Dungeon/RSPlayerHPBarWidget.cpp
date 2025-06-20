// Fill out your copyright notice in the Description page of Project Settings.


#include "RSPlayerHPBarWidget.h"
#include "RSDunPlayerController.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Blueprint/WidgetTree.h"

void URSPlayerHPBarWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

    ARSDunPlayerController* RSDunPlayerController = GetOwningPlayer<ARSDunPlayerController>();

    if (RSDunPlayerController)
    {
        RSDunPlayerController->OnHPChange.AddDynamic(this, &URSPlayerHPBarWidget::UpdateHP);
    }

    LastUpdateHP = 0.f;
    LastUpdateMaxHP = 0.f;
}

void URSPlayerHPBarWidget::NativeConstruct()
{
    Super::NativeConstruct();


}

void URSPlayerHPBarWidget::UpdateHP(float HP, float MaxHP)
{
    // HP에 대한 텍스트 수정
    if (HPText)
    {
        FString HPString = FString::Printf(TEXT("%.0f"), HP); // 소수점 없는 정수 형태로 변환
        HPText->SetText(FText::FromString(HPString));
    }

    if (MaxHPText)
    {
        FString MaxHPString = FString::Printf(TEXT("%.0f"), MaxHP); // 소수점 없는 정수 형태로 변환
        MaxHPText->SetText(FText::FromString(MaxHPString));
    }

    // 프로그레스 바의 퍼센트 변경
    if (HPProgressBar)
    {
        const float HPPercent = (HP > 0.f) ? (float)HP / MaxHP : 0.f;
        float TargetPercent = FMath::Clamp(HPPercent, 0.0f, 1.0f);  // 0 ~ 1 사이로 고정
        HPProgressBar->SetPercent(TargetPercent);
    }

    // 마지막으로 업데이트 했던 최대 체력과 다른 경우
    if (LastUpdateMaxHP != MaxHP)
    {
        // 최대 체력의 3배를 체력바의 길이로 설정합니다.
        if (HPBarSizeBox)
        {
            HPBarSizeBox->SetWidthOverride(3 * MaxHP);
        }

        // 변경된 체력을 기준으로 마커의 크기를 변경합니다.
        if (HPBarHorizontalBox && MarkerHorizontalBox)
        {


            float HPBarBoxSize = HPBarHorizontalBox->GetDesiredSize().X;

            if (HPBarBoxSize == 0)
            {
                // GetDesiredSize함수가 NativeConstruct혹은 BeginPlay에서는 동작하지 않으므로 강제로 갱신
                HPBarHorizontalBox->ForceLayoutPrepass();

                HPBarBoxSize = HPBarHorizontalBox->GetDesiredSize().X;
            }

            UCanvasPanelSlot* MarkerHorizontalSlot = Cast<UCanvasPanelSlot>(MarkerHorizontalBox->Slot);
            if (MarkerHorizontalSlot)
            {
                FVector2D TargetSize{ HPBarBoxSize, MarkerHorizontalSlot->GetSize().Y };
                MarkerHorizontalSlot->SetSize(TargetSize);
            }
        }

        // MaxHP가 커진 경우 마커 추가로 생성
        int32 MarkerCount = MaxHP / 25;
        if (MarkerCount > MarkerArray.Num())
        {
            // 마지막에 삽입될 스페이서를 임시로 빼둡니다.
            UWidget* LastInsertSpacer = MarkerHorizontalBox->GetChildAt(MarkerHorizontalBox->GetChildrenCount() - 1);
            MarkerHorizontalBox->RemoveChildAt(MarkerHorizontalBox->GetChildrenCount() - 1);

            // 체력 25마다 마커를 생성합니다.
            while (MarkerCount > MarkerArray.Num())
            {
                // 마커 용도의 이미지 생성
                UImage* NewMarker = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
                if ((MarkerArray.Num() + 1) % 2 == 0)
                {
                    NewMarker->SetBrushFromTexture(MarkerL);
                }
                else
                {
                    NewMarker->SetBrushFromTexture(MarkerS);
                }

                // 마커를 삽입하고 기본적인 설정을 합니다.
                if (NewMarker && MarkerHorizontalBox)
                {
                    // 삽입과 삽입한 슬롯에 대한 정보를 가져옵니다.
                    UPanelSlot* NewMarkerPanelSlot = MarkerHorizontalBox->AddChild(NewMarker);
                    UHorizontalBoxSlot* NewMarkerSlot = Cast<UHorizontalBoxSlot>(NewMarkerPanelSlot);

                    // 기본 설정
                    if (NewMarkerSlot)
                    {
                        NewMarkerSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
                        NewMarkerSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
                        NewMarkerSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
                    }

                    // 생성한 이미지 위젯 캐싱
                    MarkerArray.Add(NewMarker);
                }
            }

            // 임시로 빼둔 스페이서를 다시 삽입합니다.
            MarkerHorizontalBox->AddChild(LastInsertSpacer);
        }
        // MaxHP가 작아진 경우 기존 마커 제거
        else if (MarkerCount < MarkerArray.Num())
        {
            // 가장 마지막에 위치한 마커 제거
            MarkerHorizontalBox->RemoveChild(MarkerArray.Last());

            // 캐싱 했던 배열에서 제거
            MarkerArray.Pop();
        }
    }
}
