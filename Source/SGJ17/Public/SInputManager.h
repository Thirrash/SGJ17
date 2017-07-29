// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SInputManager.generated.h"

/*
USTRUCT()
struct BoolWrapper {
public:
	bool Val;

public:
	BoolWrapper(bool Value) { Val = Value; }
};
*/

UCLASS()
class SGJ17_API ASInputManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASInputManager();

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

public:	
	// Called every frame
	void Tick(float DeltaTime) override;

private:
	UPROPERTY() TArray<FString> MovementPosssibilities;
	int32 CurrentMovementIndex;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, FString> InputMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> UnusedArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float CurrentLeftEnergy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float CurrentRightEnergy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float CurrentUpEnergy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float CurrentDownEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float VerticalEnergyChange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float HorizontalEnergyChange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float EnergyRegen;
	
	bool bIsInputBlocked;

public:
	UFUNCTION(BlueprintCallable)
		void ChangeMovement(bool IsFirstTime);
	
};
