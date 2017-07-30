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
class UUserWidget;

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

	void Tick(float DeltaTime) override;
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* PlayerFlipbookComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* PlayerCameraComponent = nullptr;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnDeltaTime = 0.85;

	float HorizontalSpeed;
	float VerticalSpeed;
	UFUNCTION() void SpawnPillow();

	UFUNCTION(BlueprintCallable) void ShowAd();

	UPROPERTY(EditAnywhere, BlueprintReadWrite) TSubclassOf<UUserWidget> WidgetBP;

private:
	FVector InputRootChange;
	FVector InputSpriteChange;
	UPROPERTY()
	ASInputManager* SInputManager;

	UPROPERTY()
	UPaperFlipbook* RunFlipbook;

	UPROPERTY()
	UPaperFlipbook* IdleFlipbook;

	bool bIsFacingRight = true;
	bool bIsMoving = false;};
