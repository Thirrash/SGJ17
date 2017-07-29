// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SPlayer.generated.h"

class ASInputManager;
class UPaperSpriteComponent;
class UCameraComponent;

UCLASS()
class SGJ17_API ASPlayer : public APawn
{
	GENERATED_BODY()

public:
	ASPlayer();

protected:
	void BeginPlay() override;

public:
	void MoveVertical(float Value);
	void MoveHorizontal(float Value);

public:	
	void Tick(float DeltaTime) override;
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	FVector InputCameraChange;
	FVector InputSpriteChange;
	UPROPERTY() ASInputManager* InputManager;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent* PlayerSpriteComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* PlayerCameraComponent = nullptr;

public:
	float HorizontalSpeed;
	float VerticalSpeed;

	UFUNCTION() void SpawnPillow();
};
