// Fill out your copyright notice in the Description page of Project Settings.

#include "SPlayer.h"
#include "DEBUG/Utils.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"

ASPlayer::ASPlayer() {
	PrimaryActorTick.bCanEverTick = false;
}

void ASPlayer::BeginPlay() {
	Super::BeginPlay();
}

void ASPlayer::MoveHorizontal(float Value) {
	if (Value == 0.0f)
		return;

	LogB("Forward");
}

void ASPlayer::MoveVertical(float Value) {
	if (Value == 0.0f)
		return;

	LogB("Vert");
}

void ASPlayer::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ASPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("VerticalMovement", this, &ASPlayer::MoveVertical);
	PlayerInputComponent->BindAxis("HorizontalMovement", this, &ASPlayer::MoveHorizontal);
}

