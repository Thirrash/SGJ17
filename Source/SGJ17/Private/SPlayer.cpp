// Fill out your copyright notice in the Description page of Project Settings.

#include "SPlayer.h"
#include "PaperSpriteComponent.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "DEBUG/Utils.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"

ASPlayer::ASPlayer() : HorizontalSpeed(100.0f), VerticalSpeed(80.0f),
InputCameraChange(FVector::ZeroVector), InputSpriteChange(FVector::ZeroVector) {
	PrimaryActorTick.bCanEverTick = true;

	PlayerCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	RootComponent = PlayerCameraComponent;
	PlayerSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	PlayerSpriteComponent->SetupAttachment(RootComponent);
}

void ASPlayer::BeginPlay() {
	Super::BeginPlay();

}

void ASPlayer::MoveHorizontal(float Value) {
	if (Value == 0.0f)
		return;

	InputCameraChange += FVector(0.0f, Value * HorizontalSpeed, 0.0f);
}

void ASPlayer::MoveVertical(float Value) {
	if (Value == 0.0f)
		return;

	InputSpriteChange += FVector(0.0f, 0.0f, Value * VerticalSpeed);
}

void ASPlayer::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	SetActorLocation(
		GetActorLocation() + InputCameraChange*DeltaTime,
		true
	);

	UE_LOG(LogTemp, Warning, TEXT("input sprite change: %s"), *InputSpriteChange.ToString())
	PlayerSpriteComponent->SetWorldLocation(
		PlayerSpriteComponent->GetComponentLocation() + InputSpriteChange*DeltaTime
	);

	InputCameraChange = FVector::ZeroVector;
	InputSpriteChange = FVector::ZeroVector;
}

void ASPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("VerticalMovement", this, &ASPlayer::MoveVertical);
	PlayerInputComponent->BindAxis("HorizontalMovement", this, &ASPlayer::MoveHorizontal);
}

