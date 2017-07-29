// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SPlayer.generated.h"

class ASInputManager;
class UPaperSpriteComponent;
class UPaperFlipbookComponent;
class UPaperFlipbook;
class UCameraComponent;


UCLASS()
class SGJ17_API ASPlayer : public APawn
{
	GENERATED_BODY()

public:
	ASPlayer();

	void MoveVertical(float Value);
	void MoveHorizontal(float Value);

	void Tick(float DeltaTime) override;
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	UPaperFlipbookComponent* PlayerFlipbookComponent = nullptr;
	UPROPERTY(EditAnywhere)
	UCameraComponent* PlayerCameraComponent = nullptr;

	float HorizontalSpeed;
	float VerticalSpeed;

private:
	FVector InputCameraChange;
	FVector InputSpriteChange;
	UPROPERTY()
	ASInputManager* InputManager;

	UPROPERTY()
	UPaperFlipbook* RunFlipbook;

	UPROPERTY()
	UPaperFlipbook* IdleFlipbook;

	bool bIsFacingRight = true;
	bool bIsMoving = false;

protected:
	void BeginPlay() override;
};
