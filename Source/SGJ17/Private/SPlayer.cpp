// Fill out your copyright notice in the Description page of Project Settings.

#include "SPlayer.h"
#include "PaperSpriteComponent.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "DEBUG/Utils.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "SInputManager.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "SPillowSpawner.h"

ASPlayer::ASPlayer() : HorizontalSpeed(100.0f), VerticalSpeed(80.0f),
InputCameraChange(FVector::ZeroVector), InputSpriteChange(FVector::ZeroVector) {
	PrimaryActorTick.bCanEverTick = true;

	PlayerCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	RootComponent = PlayerCameraComponent;
	PlayerSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	PlayerSpriteComponent->SetupAttachment(RootComponent);

	LogD("Start");
}

void ASPlayer::BeginPlay() {
	Super::BeginPlay();

	TArray<AActor*> manageArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASInputManager::StaticClass(), manageArray);
	InputManager = Cast<ASInputManager>(manageArray[0]);

	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate;
	timerDelegate.BindUFunction(this, FName("SpawnPillow"));
	GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, 5.0f, true, 1.0f);
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

	if (InputManager->bIsInputBlocked) {
		InputCameraChange = FVector::ZeroVector;
		InputSpriteChange = FVector::ZeroVector;
		return;
	}

	InputCameraChange *= DeltaTime;
	InputSpriteChange *= DeltaTime;

	SetActorLocation(
		GetActorLocation() + InputCameraChange,
		true
	);

	if (InputCameraChange.Y > SMALL_NUMBER) {
		InputManager->CurrentRightEnergy = FMath::Clamp(
			InputManager->CurrentRightEnergy - InputManager->HorizontalEnergyChange * InputCameraChange.Y,
			0.0f, 1.0f);
	} else {
		InputManager->CurrentLeftEnergy = FMath::Clamp(
			InputManager->CurrentLeftEnergy + InputManager->HorizontalEnergyChange * InputCameraChange.Y,
			0.0f, 1.0f);
	}

	if (InputSpriteChange.Z > SMALL_NUMBER) {
		InputManager->CurrentUpEnergy = FMath::Clamp(
			InputManager->CurrentUpEnergy - InputManager->VerticalEnergyChange * InputSpriteChange.Z,
			0.0f, 1.0f);
	} else {
		InputManager->CurrentDownEnergy = FMath::Clamp(
			InputManager->CurrentDownEnergy + InputManager->VerticalEnergyChange * InputSpriteChange.Z,
			0.0f, 1.0f);
	}

	PlayerSpriteComponent->SetWorldLocation(
		PlayerSpriteComponent->GetComponentLocation() + InputSpriteChange
	);

	InputCameraChange = FVector::ZeroVector;
	InputSpriteChange = FVector::ZeroVector;
}

void ASPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("VerticalMovement", this, &ASPlayer::MoveVertical);
	PlayerInputComponent->BindAxis("HorizontalMovement", this, &ASPlayer::MoveHorizontal);
}

void ASPlayer::SpawnPillow() {
	USPillowSpawner* spawner = FindComponentByClass<USPillowSpawner>();
	Check(spawner);

	if (spawner->bIsFirstLevel)
		return;

	FTransform transform;
	float sign = ((bool)FMath::RandRange(0, 1)) ? 1.0f : -1.0f;
	transform.SetLocation(FVector(
		PlayerSpriteComponent->GetComponentLocation().X, 
		PlayerSpriteComponent->GetComponentLocation().Y + sign * 360.0f,
		PlayerSpriteComponent->GetComponentLocation().Z));
	spawner->SpawnNewPillow(transform);
}