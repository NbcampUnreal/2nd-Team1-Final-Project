// Fill out your copyright notice in the Description page of Project Settings.


#include "RSGameInstance.h"
#include "RogShop/UtilDefine.h"

bool URSGameInstance::GetDebugLogEnabled() const
{
	return bEnableDebugLog;
}

void URSGameInstance::SetDebugLogEnabled(bool bEnable)
{
	bEnableDebugLog = bEnable;
}
