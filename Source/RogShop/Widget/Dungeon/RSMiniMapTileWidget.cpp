// Fill out your copyright notice in the Description page of Project Settings.


#include "RSMiniMapTileWidget.h"
#include "Components/Border.h"

// 위젯의 상태(방문, 현재 위치, 보스, 상점) 설정
void URSMiniMapTileWidget::SetState(bool bVisitedIn, bool bCurrentIn, bool bBossIn)
{
	bIsVisited = bVisitedIn;
	bIsCurrent = bCurrentIn;
	bIsBossRoom = bBossIn;

	UpdateStyle();
}

// 타일의 상태에 따라 색상을 설정
void URSMiniMapTileWidget::UpdateStyle()
{
	FLinearColor Color = FLinearColor::Transparent; // 기본은 투명

	if (bIsCurrent)
		Color = FLinearColor::Green; // 현재 위치는 초록
	else if (bIsBossRoom)
		Color = FLinearColor::Red;  // 보스방은 빨강
	else if (bIsVisited)
		Color = FLinearColor(0.4f, 0.4f, 0.4f); // 회색 (방문)

	// UMG Border에 색상 적용
	if (TileBorder)
		TileBorder->SetBrushColor(Color);
}
