// Fill out your copyright notice in the Description page of Project Settings.

#include "SInputManager.h"
#include "Runtime/Engine/Classes/GameFramework/InputSettings.h"
#include "DEBUG/Utils.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectIterator.h"
#include <string>

// Sets default values
ASInputManager::ASInputManager() :
MovementPosssibilities(TArray<FString>()), InputMap(TMap<FString, FString>()) {
	PrimaryActorTick.bCanEverTick = true;

	MovementPosssibilities = { "WASD", "ESDF", "RDFG" };
	CurrentMovementIndex = 0;

	bIsInputBlocked = false;

	CurrentLeftEnergy = 1.0f;
	CurrentDownEnergy = 1.0f;
	CurrentRightEnergy = 1.0f;
	CurrentUpEnergy = 1.0f;

	EnergyRegen = 0.15f;

	VerticalEnergyChange = 0.002f;
	HorizontalEnergyChange = 0.0003f;

	InputMap.Add("VerticalMovementPositive", "W");
	InputMap.Add("VerticalMovementNegative", "S");
	InputMap.Add("HorizontalMovementPositive", "D");
	InputMap.Add("HorizontalMovementNegative", "A");
}

void ASInputManager::BeginPlay() {
	Super::BeginPlay();

	ChangeMovement(true);
}

void ASInputManager::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (CheckIfAllEnergiesEmpty()) {
		bIsInputBlocked = true;
		FTimerHandle timerHandle;
		FTimerDelegate timerDelegate;
		timerDelegate.BindLambda([&]() { bIsInputBlocked = false; ChangeMovement(false); });
		GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, 1.5f, false);
	}

	CurrentLeftEnergy = FMath::Clamp(CurrentLeftEnergy + EnergyRegen * DeltaTime, 0.0f, 1.0f);
	CurrentDownEnergy = FMath::Clamp(CurrentDownEnergy + EnergyRegen * DeltaTime, 0.0f, 1.0f);
	CurrentRightEnergy = FMath::Clamp(CurrentRightEnergy + EnergyRegen * DeltaTime, 0.0f, 1.0f);
	CurrentUpEnergy = FMath::Clamp(CurrentUpEnergy + EnergyRegen * DeltaTime, 0.0f, 1.0f);
}

bool ASInputManager::CheckIfAllEnergiesEmpty()
{
	return CurrentDownEnergy == 0.0f || CurrentLeftEnergy == 0.0f || CurrentRightEnergy == 0.0f || CurrentUpEnergy == 0.0f;
}

void ASInputManager::ChangeMovement(bool IsFirstTime) {
	int32 randElement;
	do {
		randElement = FMath::RandRange(0, MovementPosssibilities.Num() - 1);
	} while (randElement == CurrentMovementIndex);
	CurrentMovementIndex = randElement;

	if (IsFirstTime) {
		CurrentMovementIndex = 0;
		randElement = 0;
	}

	UInputSettings* settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	TArray<FInputAxisKeyMapping>& axesInputsMapping = settings->AxisMappings;

	for (FInputAxisKeyMapping& each : axesInputsMapping) {
		if (each.AxisName.ToString() == FString("VerticalMovement")) {
			if (each.Scale > 0.0f) {
				FString keyName = FString(std::string(TCHAR_TO_UTF8(*(MovementPosssibilities[randElement]))).substr(0, 1).c_str());
				each.Key = FKey(*keyName);
				InputMap["VerticalMovementPositive"] = keyName;
			} else {
				FString keyName = FString(std::string(TCHAR_TO_UTF8(*(MovementPosssibilities[randElement]))).substr(2, 1).c_str());
				each.Key = FKey(*keyName);
				InputMap["VerticalMovementNegative"] = keyName;
			}
		} else if (each.AxisName.ToString() == FString("HorizontalMovement")) {
			if (each.Scale > 0.0f) {
				FString keyName = FString(std::string(TCHAR_TO_UTF8(*(MovementPosssibilities[randElement]))).substr(3, 1).c_str());
				each.Key = FKey(*keyName);
				InputMap["HorizontalMovementPositive"] = keyName;
			} else {
				FString keyName = FString(std::string(TCHAR_TO_UTF8(*(MovementPosssibilities[randElement]))).substr(1, 1).c_str());
				each.Key = FKey(*keyName);
				InputMap["HorizontalMovementNegative"] = keyName;
			}
		}
	}
	
	const_cast<UInputSettings*>(settings)->SaveKeyMappings();
	for (TObjectIterator<UPlayerInput> It; It; ++It)
		It->ForceRebuildingKeyMaps(true);
}
