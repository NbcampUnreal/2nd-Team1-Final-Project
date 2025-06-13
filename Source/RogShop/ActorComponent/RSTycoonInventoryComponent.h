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
	
	void UpdateInventoryWidget();
	
	UFUNCTION()
	virtual void SaveItemData() override;
	virtual void LoadItemData() override;
	

	
	bool IsOpen() const { return bIsOpen; };

protected:
	virtual void BeginPlay() override;
	
private:
	bool bIsOpen;
};
