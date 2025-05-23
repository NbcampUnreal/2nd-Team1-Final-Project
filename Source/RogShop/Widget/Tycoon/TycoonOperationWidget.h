// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TycoonOperationWidget.generated.h"

class ARSTycoonWaiterCharacter;
class ARSTycoonChefCharacter;
class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class ROGSHOP_API UTycoonOperationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetGold(int32 Value);

	virtual void NativeConstruct() override;
	
private:
	UFUNCTION()
	void CreateChef();
	
	UFUNCTION()
	void CreateWaiter();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* GoldText;

	//== 시연용 == 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UButton* CreateChefButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UButton* CreateWaiterButton;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARSTycoonChefCharacter> ChefClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARSTycoonWaiterCharacter> WaiterClass;

	UPROPERTY(EditDefaultsOnly)
	FVector PlusSpawnLo;	
};
