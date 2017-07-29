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
class UStaticMeshConponent;


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
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent = nullptr;

	float HorizontalSpeed;
	float VerticalSpeed;

private:
	FVector InputRootChange;
	FVector InputSpriteChange;
	UPROPERTY()
	ASInputManager* InputManager;

	UPROPERTY()
	UPaperFlipbook* RunFlipbook;

	UPROPERTY()
	UPaperFlipbook* IdleFlipbook;

	void CollisionTick(float DeltaTime);

	float CalcOffsetScaleAfterChildSweep();

	bool bIsFacingRight = true;
	bool bIsMoving = false;

protected:
	void BeginPlay() override;
};
