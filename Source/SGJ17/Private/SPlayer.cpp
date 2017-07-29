// Fill out your copyright notice in the Description page of Project Settings.

#include "SPlayer.h"
#include "DEBUG/Utils.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"

ASPlayer::ASPlayer() : HorizontalSpeed(1.0f), VerticalSpeed(1.0f),
InputChange(FVector::ZeroVector) {
	PrimaryActorTick.bCanEverTick = true;
}

void ASPlayer::BeginPlay() {
	Super::BeginPlay();
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

	SetActorLocation(
		GetActorLocation() + InputChange,
		true
	);

	InputChange = FVector::ZeroVector;
}

void ASPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("VerticalMovement", this, &ASPlayer::MoveVertical);
	PlayerInputComponent->BindAxis("HorizontalMovement", this, &ASPlayer::MoveHorizontal);
}

