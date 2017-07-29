// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPillow.generated.h"

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

	
};
