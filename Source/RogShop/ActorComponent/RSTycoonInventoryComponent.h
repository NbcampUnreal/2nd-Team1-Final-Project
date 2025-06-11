// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RSBaseInventoryComponent.h"
#include "RSTycoonInventoryComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROGSHOP_API URSTycoonInventoryComponent : public URSBaseInventoryComponent
{
	GENERATED_BODY()

public:
	URSTycoonInventoryComponent();

	UFUNCTION(BlueprintCallable)
	void Open();
	UFUNCTION(BlueprintCallable)
	void Close();
	UFUNCTION(BlueprintCallable, CallInEditor)
	virtual void SaveItemData() override;
	
	virtual void LoadItemData() override;
	
	void SetInventorySlot(int32 Value);
	
	bool IsOpen() const { return bIsOpen; };

private:
	bool bIsOpen;
};
