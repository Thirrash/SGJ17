// Fill out your copyright notice in the Description page of Project Settings.

#include "SPlayer.h"
#include "PaperSpriteComponent.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "DEBUG/Utils.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "SInputManager.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "ConstructorHelpers.h"

ASPlayer::ASPlayer() : HorizontalSpeed(100.0f), VerticalSpeed(80.0f),
InputCameraChange(FVector::ZeroVector), InputSpriteChange(FVector::ZeroVector) {
	PrimaryActorTick.bCanEverTick = true;

	PlayerCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	RootComponent = PlayerCameraComponent;
	PlayerFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));

	ConstructorHelpers::FObjectFinder<UPaperFlipbook> RunFlipbookObj(TEXT("/Game/Flipbooks/walk/walk_example_Walk_Flipbook.walk_example_Walk_Flipbook"));
	ConstructorHelpers::FObjectFinder<UPaperFlipbook> IdleFlipbookObj(TEXT("/Game/Flipbooks/idle/idle_Flipbook.idle_Flipbook"));
	RunFlipbook = RunFlipbookObj.Object;
	IdleFlipbook = IdleFlipbookObj.Object;

	PlayerFlipbookComponent->SetFlipbook(IdleFlipbook);
	PlayerFlipbookComponent->SetupAttachment(RootComponent);
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

	bool bIsMovingNow = false;
	if (InputCameraChange.Y > SMALL_NUMBER) {
		InputManager->CurrentRightEnergy = FMath::Clamp(
			InputManager->CurrentRightEnergy - InputManager->HorizontalEnergyChange * InputCameraChange.Y,
			0.0f, 1.0f);
		bIsFacingRight = true;
		bIsMovingNow = true;
	} else if (InputCameraChange.Y < -SMALL_NUMBER) {
		InputManager->CurrentLeftEnergy = FMath::Clamp(
			InputManager->CurrentLeftEnergy + InputManager->HorizontalEnergyChange * InputCameraChange.Y,
			0.0f, 1.0f);
		bIsFacingRight = false;
		bIsMovingNow = true;
	}

	if (InputSpriteChange.Z > SMALL_NUMBER) {
		InputManager->CurrentUpEnergy = FMath::Clamp(
			InputManager->CurrentUpEnergy - InputManager->VerticalEnergyChange * InputSpriteChange.Z,
			0.0f, 1.0f);
		bIsMovingNow = true;
	} else if (InputSpriteChange.Z < -SMALL_NUMBER) {
		InputManager->CurrentDownEnergy = FMath::Clamp(
			InputManager->CurrentDownEnergy + InputManager->VerticalEnergyChange * InputSpriteChange.Z,
			0.0f, 1.0f);
		bIsMovingNow = true;
	}
	
	if (bIsMovingNow) {
		if (!bIsMoving) {
			PlayerFlipbookComponent->SetFlipbook(RunFlipbook);
			bIsMoving = true;
		}
	}
	else if (bIsMoving) {
		PlayerFlipbookComponent->SetFlipbook(IdleFlipbook);
		bIsMoving = false;
	}

	if (bIsFacingRight) {
		PlayerFlipbookComponent->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	}
	else {
		PlayerFlipbookComponent->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	}

	if (InputManager->CheckIfAllEnergiesEmpty()) {
		PlayerFlipbookComponent->SetFlipbook(IdleFlipbook);
	}

	PlayerFlipbookComponent->SetWorldLocation(
		PlayerFlipbookComponent->GetComponentLocation() + InputSpriteChange
	);

	InputCameraChange = FVector::ZeroVector;
	InputSpriteChange = FVector::ZeroVector;
}

void ASPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("VerticalMovement", this, &ASPlayer::MoveVertical);
	PlayerInputComponent->BindAxis("HorizontalMovement", this, &ASPlayer::MoveHorizontal);
}

