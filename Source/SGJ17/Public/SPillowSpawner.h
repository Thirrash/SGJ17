// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <vector>
#include <map>
#include "SPillowSpawner.generated.h"

class UPaperSpriteComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SGJ17_API USPillowSpawner : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USPillowSpawner();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	std::map<int32, bool> SpawnedMap;
	std::vector<char> Keys;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UPaperSpriteComponent* PlayerSpriteComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ASPillow> PillowBPClass;

	bool bIsFirstLevel = false;
	int32 noSpawned;

public:
	UFUNCTION(BlueprintCallable) void SpawnNewPillow(FTransform Transform);
	
};
