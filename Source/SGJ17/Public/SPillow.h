// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPillow.generated.h"

class ASPlayer;

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

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MovementSpeed;

public:
	void InitPlayer(ASPlayer* CurrentPlayer);
	void InitName(int PillowNr, char KeyRequired);

	void OnPillowHit();
};
