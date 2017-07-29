// Fill out your copyright notice in the Description page of Project Settings.

#include "SPillow.h"
#include "DEBUG/Utils.h"
#include "SPlayer.h"
#include "PaperSpriteComponent.h"
#include "Runtime/Engine/Classes/GameFramework/InputSettings.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectIterator.h"

// Sets default values
ASPillow::ASPillow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MovementSpeed = 1.0f;
}

// Called when the game starts or when spawned
void ASPillow::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void ASPillow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(FMath::VInterpTo(GetActorLocation(), Player->PlayerSpriteComponent->GetComponentLocation(), DeltaTime, MovementSpeed));
	if (FVector::Distance(GetActorLocation(), Player->PlayerSpriteComponent->GetComponentLocation()) < 5.0f) {
		//OnPillowHit();
		//LogA("HitPlayer");
		Destroy();
	}
}

void ASPillow::InitPlayer(ASPlayer * CurrentPlayer) {
	if (BCheck(CurrentPlayer))
		Player = CurrentPlayer;
}

void ASPillow::InitName(int PillowNr, char KeyRequired) {

	std::string name = "Pillow";
	name += std::to_string(PillowNr);
	Key = KeyRequired;
	
	UInputSettings* settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	Check(settings);
	FString keyName = FString(std::string(1, KeyRequired).c_str());
	BindingName = FString(name.c_str());
	LogD(BindingName);

	TArray<FInputActionKeyMapping>& inputs = settings->ActionMappings;
	for (FInputActionKeyMapping& each : inputs) {
		LogA(each.ActionName.ToString());
		if (each.ActionName.ToString() == BindingName) {
			LogC("GUD");
			each.Key = FKey(*keyName);
		}
	}

	const_cast<UInputSettings*>(settings)->SaveKeyMappings();
	for (TObjectIterator<UPlayerInput> It; It; ++It)
		It->ForceRebuildingKeyMaps(true);

	FInputActionBinding binding = (GetWorld()->GetFirstPlayerController()->GetPawn())->InputComponent->BindAction(
		*BindingName, EInputEvent::IE_Pressed, this, &ASPillow::OnPillowHit
	);
}

void ASPillow::OnPillowHit() {
	UInputSettings* settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	TArray<FInputActionKeyMapping>& inputs = settings->ActionMappings;
	for (FInputActionKeyMapping& each : inputs) {
		if (each.ActionName.ToString() == BindingName) {
			each.Key = FKey();
		}
	}

	const_cast<UInputSettings*>(settings)->SaveKeyMappings();
	for (TObjectIterator<UPlayerInput> It; It; ++It)
		It->ForceRebuildingKeyMaps(true);

	LogC("Hit");
	Destroy();
}