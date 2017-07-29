// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPillow.generated.h"

class ASPlayer;
class UUserWidget;
class USPillowSpawner;

UCLASS()
class SGJ17_API ASPillow : public AActor
{
	GENERATED_BODY()
	
public:	
	ASPillow();

protected:
	void BeginPlay() override;

public:	
	void Tick(float DeltaTime) override;

private:
	UPROPERTY() ASPlayer* Player;
	UPROPERTY() FString BindingName;
	char Key;
	UPROPERTY() USPillowSpawner* PillowSpawner;
	int32 PillowNumber;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString KeyString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UUserWidget* InputWidget;

public:
	void InitPlayer(ASPlayer* CurrentPlayer);
	void InitName(int PillowNr, char KeyRequired, USPillowSpawner* Spawner);

	void OnPillowHit();
};
