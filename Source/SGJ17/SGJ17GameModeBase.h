// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGJ17GameModeBase.generated.h"

UENUM()
enum class ESButtonState : uint8 {
	Normal 			UMETA(DisplayName = "Normal"),
	Blocked 		UMETA(DisplayName = "Blocked"),
	Killing 		UMETA(DisplayName = "Killing")
};

/**
 * 
 */
UCLASS()
class SGJ17_API ASGJ17GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASGJ17GameModeBase();
	


public:
	UFUNCTION(BlueprintCallable, Category = Game)
		void StartPlay() override;
	
};
