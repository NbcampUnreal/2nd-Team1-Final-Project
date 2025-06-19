// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RSTycoonEvent.generated.h"

class URSTycoonEventViewWidget;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ROGSHOP_API URSTycoonEvent : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	bool Condition(UWorld* World);
	UFUNCTION(BlueprintNativeEvent)
	void Fail(UWorld* World);
	UFUNCTION(BlueprintNativeEvent)
	void Success(UWorld* World);

protected:
	UFUNCTION(BlueprintCallable)
	AActor* GetActorOfClass(UWorld* World, TSubclassOf<AActor> ActorClass);
	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetAllActorsOfClass(UWorld* World, TSubclassOf<AActor> ActorClass);
	UFUNCTION(BlueprintPure)
	APlayerController* GetPlayerController(UWorld* World);
	UFUNCTION(BlueprintPure)
	AGameModeBase* GetGameModeBase(UWorld* World);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Priority;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EventName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(MultiLine=true))
	FString Description;

	
};
