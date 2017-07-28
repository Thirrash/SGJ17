// Fill out your copyright notice in the Description page of Project Settings.

#include "SGJ17GameModeBase.h"
#include "Runtime/Engine/Classes/GameFramework/InputSettings.h"
#include "DEBUG/Utils.h"

ASGJ17GameModeBase::ASGJ17GameModeBase() : AGameModeBase(),
InputMap(TMap<FString, ESButtonState>()) {

}

void ASGJ17GameModeBase::StartPlay() {
	const UInputSettings* settings = GetDefault<UInputSettings>();
	const TArray<FInputAxisKeyMapping>& axesInputsMapping = settings->AxisMappings;

	if (axesInputsMapping.FindByPredicate([](FInputAxisKeyMapping f) { f.AxisName == FName("VerticalMovement"); }))
		LogC("Gud");

	Super::StartPlay();
}
