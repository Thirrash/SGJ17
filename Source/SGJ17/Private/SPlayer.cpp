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
#include  "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "SPillowSpawner.h"

ASPlayer::ASPlayer() : HorizontalSpeed(100.0f), VerticalSpeed(80.0f),
InputRootChange(FVector::ZeroVector), InputSpriteChange(FVector::ZeroVector) {
	PrimaryActorTick.bCanEverTick = true;

	PlayerCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	PlayerFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

	RootComponent = MeshComponent;
	PlayerCameraComponent->SetupAttachment(RootComponent);
	PlayerFlipbookComponent->SetupAttachment(PlayerCameraComponent);

	ConstructorHelpers::FObjectFinder<UPaperFlipbook> RunFlipbookObj(TEXT("PaperFlipbook'/Game/Image/bezp³odny_walk_CharacterWalk.bezp³odny_walk_CharacterWalk'"));
	ConstructorHelpers::FObjectFinder<UPaperFlipbook> IdleFlipbookObj(TEXT("PaperFlipbook'/Game/Image/CharacterIdle.CharacterIdle'"));
	RunFlipbook = RunFlipbookObj.Object;
	IdleFlipbook = IdleFlipbookObj.Object;

	PlayerFlipbookComponent->SetFlipbook(IdleFlipbook);
}


void ASPlayer::BeginPlay() {
	Super::BeginPlay();

	TArray<AActor*> manageArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASInputManager::StaticClass(), manageArray);
	SInputManager = Cast<ASInputManager>(manageArray[0]);

	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate;
	timerDelegate.BindUFunction(this, FName("SpawnPillow"));
	GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, 5.0f, true, 1.0f);
}

void ASPlayer::MoveHorizontal(float Value) {
	if (Value == 0.0f)
		return;

	InputRootChange += FVector(0.0f, Value * HorizontalSpeed, 0.0f);
}

void ASPlayer::MoveVertical(float Value) {
	if (Value == 0.0f)
		return;

	InputSpriteChange += FVector(0.0f, 0.0f, Value * VerticalSpeed);
}

void ASPlayer::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (SInputManager->bIsInputBlocked) {
		InputRootChange = FVector::ZeroVector;
		InputSpriteChange = FVector::ZeroVector;
		return;
	}

	InputRootChange *= DeltaTime;
	InputSpriteChange *= DeltaTime;

	FHitResult* hit = new FHitResult();
	SetActorLocation(GetActorLocation() + InputRootChange + InputSpriteChange, true, hit);

	PlayerCameraComponent->SetWorldLocation(PlayerCameraComponent->GetComponentLocation() - InputSpriteChange * hit->Time);
	PlayerFlipbookComponent->SetWorldLocation(PlayerFlipbookComponent->GetComponentLocation() + InputSpriteChange * hit->Time);
	delete hit;

	bool bIsMovingNow = false;
	if (InputRootChange.Y > SMALL_NUMBER) {
		SInputManager->CurrentRightEnergy = FMath::Clamp(
			SInputManager->CurrentRightEnergy - SInputManager->HorizontalEnergyChange * InputRootChange.Y,
			0.0f, 1.0f);
		bIsFacingRight = true;
		bIsMovingNow = true;
	} else if (InputRootChange.Y < -SMALL_NUMBER) {
		SInputManager->CurrentLeftEnergy = FMath::Clamp(
			SInputManager->CurrentLeftEnergy + SInputManager->HorizontalEnergyChange * InputRootChange.Y,
			0.0f, 1.0f);
		bIsFacingRight = false;
		bIsMovingNow = true;
	}

	if (InputSpriteChange.Z > SMALL_NUMBER) {
		SInputManager->CurrentUpEnergy = FMath::Clamp(
			SInputManager->CurrentUpEnergy - SInputManager->VerticalEnergyChange * InputSpriteChange.Z,
			0.0f, 1.0f);
		bIsMovingNow = true;
	} else if (InputSpriteChange.Z < -SMALL_NUMBER) {
		SInputManager->CurrentDownEnergy = FMath::Clamp(
			SInputManager->CurrentDownEnergy + SInputManager->VerticalEnergyChange * InputSpriteChange.Z,
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

	if (SInputManager->CheckIfAllEnergiesEmpty()) {
		PlayerFlipbookComponent->SetFlipbook(IdleFlipbook);
	}

	InputRootChange = FVector::ZeroVector;
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
		PlayerFlipbookComponent->GetComponentLocation().X,
		PlayerFlipbookComponent->GetComponentLocation().Y + sign * 360.0f,
		PlayerFlipbookComponent->GetComponentLocation().Z));
	spawner->SpawnNewPillow(transform);
}
