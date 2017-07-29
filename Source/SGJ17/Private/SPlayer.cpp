// Fill out your copyright notice in the Description page of Project Settings.

#include "SPlayer.h"
#include "DEBUG/Utils.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "SInputManager.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"

ASPlayer::ASPlayer() : HorizontalSpeed(100.0f), VerticalSpeed(100.0f),
InputChange(FVector::ZeroVector) {
	PrimaryActorTick.bCanEverTick = true;
}

void ASPlayer::BeginPlay() {
	Super::BeginPlay();

	TArray<AActor*> manageArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASInputManager::StaticClass(), manageArray);
	InputManager = Cast<ASInputManager>(manageArray[0]);
}

void ASPlayer::MoveHorizontal(float Value) {
	if (Value == 0.0f)
		return;

	InputChange += FVector(0.0f, Value * HorizontalSpeed, 0.0f);
}

void ASPlayer::MoveVertical(float Value) {
	if (Value == 0.0f)
		return;

	InputChange += FVector(0.0f, 0.0f, Value * VerticalSpeed);
}

void ASPlayer::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (InputManager->bIsInputBlocked) {
		InputChange = FVector::ZeroVector;
		return;
	}

	InputChange *= DeltaTime;

	SetActorLocation(
		GetActorLocation() + InputChange,
		true
	);

	if (InputChange.Y > SMALL_NUMBER) {
		InputManager->CurrentRightEnergy = FMath::Clamp(
			InputManager->CurrentRightEnergy - InputManager->HorizontalEnergyChange * InputChange.Y,
			0.0f, 1.0f);
	} else {
		InputManager->CurrentLeftEnergy = FMath::Clamp(
			InputManager->CurrentLeftEnergy + InputManager->HorizontalEnergyChange * InputChange.Y,
			0.0f, 1.0f);
	}

	if (InputChange.Z > SMALL_NUMBER) {
		InputManager->CurrentUpEnergy = FMath::Clamp(
			InputManager->CurrentUpEnergy - InputManager->VerticalEnergyChange * InputChange.Z,
			0.0f, 1.0f);
	} else {
		InputManager->CurrentDownEnergy = FMath::Clamp(
			InputManager->CurrentDownEnergy + InputManager->VerticalEnergyChange * InputChange.Z,
			0.0f, 1.0f);
	}

	InputChange = FVector::ZeroVector;
}

void ASPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("VerticalMovement", this, &ASPlayer::MoveVertical);
	PlayerInputComponent->BindAxis("HorizontalMovement", this, &ASPlayer::MoveHorizontal);
}

