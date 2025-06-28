// Fill out your copyright notice in the Description page of Project Settings.


#include "RSGameInstance.h"
#include "RogShop/UtilDefine.h"
#include "Sound/SoundClass.h"

bool URSGameInstance::GetDebugLogEnabled() const
{
	return bEnableDebugLog;
}

void URSGameInstance::SetDebugLogEnabled(bool bEnable)
{
	bEnableDebugLog = bEnable;
}

void URSGameInstance::OnMasterVolumeChanged(float Value)
{
	if (SC_Master)
	{
		SC_Master->Properties.Volume = Value;
	}
}

void URSGameInstance::OnBGMVolumeChanged(float Value)
{
	if (SC_BGM)
	{
		SC_BGM->Properties.Volume = Value;
	}
}

void URSGameInstance::OnSFXVolumeChanged(float Value)
{
	if (SC_SFX)
	{
		SC_SFX->Properties.Volume = Value;
	}
}
