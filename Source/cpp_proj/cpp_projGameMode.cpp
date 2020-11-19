// Copyright Epic Games, Inc. All Rights Reserved.

#include "cpp_projGameMode.h"
#include "cpp_projCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

Acpp_projGameMode::Acpp_projGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	FStringAssetReference itemRef = "Blueprint'/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter.ThirdPersonCharacter'";
	UObject* itemObj = itemRef.ResolveObject();
	ActorToSpawn = Cast<UBlueprint>(itemObj);

	//itemRef = "ParticleSystem'/Game/StarterContent/Particles/P_Steam_Lit.P_Steam_Lit'";
	//itemObj = itemRef.ResolveObject();
	//ParticlesToSpawn = Cast<UParticleSystem>(itemObj);

}

void Acpp_projGameMode::RespawnPlayer()
{
	if (ActorToSpawn == nullptr) return;

	APlayerController* player = GetWorld()->GetFirstPlayerController();
	FVector playerLocation;
	FRotator quat;
	if (player != nullptr)
	{
		quat = player->GetPawn()->GetActorRotation();
		playerLocation = player->GetPawn()->GetActorLocation();
		player->GetPawn()->Destroy();
	}
	const FActorSpawnParameters spawnParameters;

	//spawn particules
	//if (ParticlesToSpawn != nullptr)
	//{
	//	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticlesToSpawn, playerLocation);
	//}

	//delay
	const FVector respawnLocation = respawnPosition->GetDefaultAttachComponent()->GetComponentLocation();
	const FRotator respawnRotation = respawnPosition->GetDefaultAttachComponent()->GetComponentRotation();
	//spawn new player

	AActor* newPlayer = GetWorld()->SpawnActor<AActor>(ActorToSpawn->GeneratedClass, respawnLocation, respawnRotation, spawnParameters);

	APawn* newPlayerPawn = Cast<APawn>(newPlayer);

	player->Possess(newPlayerPawn);
	DefaultPawnClass = newPlayerPawn->GetClass();
}

void Acpp_projGameMode::SetRespawnPoint(AActor* respawnPoint)
{
	respawnPosition = respawnPoint;
}

