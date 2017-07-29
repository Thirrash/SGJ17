// Fill out your copyright notice in the Description page of Project Settings.

#include "SPillowSpawner.h"
#include "SPillow.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "SPlayer.h"
#include "ConstructorHelpers.h"
#include "DEBUG/Utils.h"

USPillowSpawner::USPillowSpawner() : SpawnedMap(std::map<int32, bool>()), Keys(std::vector<char>()),
WordMap(std::map<char, FString>()) {
	PrimaryComponentTick.bCanEverTick = true;

	SpawnedMap.insert(std::pair<int32, bool>(1, false));
	SpawnedMap.insert(std::pair<int32, bool>(2, false));
	SpawnedMap.insert(std::pair<int32, bool>(3, false));
	SpawnedMap.insert(std::pair<int32, bool>(4, false));
	SpawnedMap.insert(std::pair<int32, bool>(5, false));
	SpawnedMap.insert(std::pair<int32, bool>(6, false));
	SpawnedMap.insert(std::pair<int32, bool>(7, false));
	SpawnedMap.insert(std::pair<int32, bool>(8, false));

	Keys = { 'I', 'O', 'P', 'K', 'L', 'C', 'B', 'N' };
	noSpawned = 0;

	static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/MySPillow_BP.MySPillow_BP'"));
	if (BCheck(ItemBlueprint.Object)) {
		PillowBPClass = (UClass*)ItemBlueprint.Object->GeneratedClass;
	}
}

void USPillowSpawner::BeginPlay() {
	Super::BeginPlay();
}

void USPillowSpawner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USPillowSpawner::SpawnNewPillow(FTransform Transform) {
	std::vector<int32> pillowNrs;
	int32 pillowNr;
	for (std::pair<int32, bool> each : SpawnedMap) {
		if (each.second == false) {
			pillowNrs.push_back(each.first);
		}
	}

	pillowNr = pillowNrs[FMath::RandRange(0, pillowNrs.size() - 1)];
	SpawnedMap[pillowNr] = true;
	
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Check(GetWorld(), GetOwner());
	ASPillow* newPillow = GetWorld()->SpawnActor<ASPillow>(PillowBPClass, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);
	if (!BCheck(newPillow))
		return;

	newPillow->SetActorTransform(Transform);
	newPillow->InitPlayer(Cast<ASPlayer>(GetOwner()));
	newPillow->InitName(pillowNr, Keys[pillowNr - 1], this);

	noSpawned++;
}

void USPillowSpawner::ResetPillow(int32 nr) {
	SpawnedMap[nr] = false;
}

