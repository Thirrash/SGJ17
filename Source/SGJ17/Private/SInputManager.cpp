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
	PrimaryActorTick.bCanEverTick = false;

	MovementPosssibilities = { "WASD", "ESDF", "RDFG" };

	InputMap.Add("VerticalMovementPositive", "W");
	InputMap.Add("VerticalMovementNegative", "S");
	InputMap.Add("HorizontalMovementPositive", "A");
	InputMap.Add("HorizontalMovementNegative", "D");
}

// Called when the game starts or when spawned
void ASInputManager::BeginPlay() {
	Super::BeginPlay();

	FTimerHandle movementHandle;
	FTimerDelegate movementDelegate;
	movementDelegate.BindUFunction(this, "ChangeMovement");
	GetWorld()->GetTimerManager().SetTimer(movementHandle, movementDelegate, 3.0f, true);
}

// Called every frame
void ASInputManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ASInputManager::ChangeMovement() {
	int32 randElement = FMath::RandRange(0, MovementPosssibilities.Num() - 1);
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
				FString keyName = FString(std::string(TCHAR_TO_UTF8(*(MovementPosssibilities[randElement]))).substr(1, 1).c_str());
				each.Key = FKey(*keyName);
				InputMap["HorizontalMovementPositive"] = keyName;
			} else {
				FString keyName = FString(std::string(TCHAR_TO_UTF8(*(MovementPosssibilities[randElement]))).substr(3, 1).c_str());
				each.Key = FKey(*keyName);
				InputMap["HorizontalMovementNegative"] = keyName;
			}
		}
	}
	
	const_cast<UInputSettings*>(settings)->SaveKeyMappings();
	for (TObjectIterator<UPlayerInput> It; It; ++It)
		It->ForceRebuildingKeyMaps(true);
}
