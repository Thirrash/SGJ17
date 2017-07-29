// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SInputManager.generated.h"

UCLASS()
class SGJ17_API ASInputManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASInputManager();

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

public:	
	// Called every frame
	void Tick(float DeltaTime) override;

private:
	UPROPERTY() TArray<FString> MovementPosssibilities;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, FString> InputMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> UnusedArray;

public:
	UFUNCTION()
		void ChangeMovement();
	
};
