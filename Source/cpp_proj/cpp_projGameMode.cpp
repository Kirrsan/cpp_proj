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

	//get path to the blueprint of the player character in our project
	FStringAssetReference itemRef = "Blueprint'/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter.ThirdPersonCharacter'";
	UObject* itemObj = itemRef.ResolveObject();
	ActorToSpawn = Cast<UBlueprint>(itemObj);

	//path to particles blueprint - did not have time to come back to it :(

	//itemRef = "ParticleSystem'/Game/StarterContent/Particles/P_Steam_Lit.P_Steam_Lit'";
	//itemObj = itemRef.ResolveObject();
	//ParticlesToSpawn = Cast<UParticleSystem>(itemObj);

}

void Acpp_projGameMode::RespawnPlayer()
{
	//if we did not find a player character blueprint, return
	if (ActorToSpawn == nullptr) return;

	//get the player controller
	APlayerController* player = GetWorld()->GetFirstPlayerController();
	//get the current player position and rotation for spawning the particles
	FVector playerLocation;
	FRotator quat;
	if (player != nullptr)
	{
		quat = player->GetPawn()->GetActorRotation();
		playerLocation = player->GetPawn()->GetActorLocation();
		//destroy player
		player->GetPawn()->Destroy();
	}
	const FActorSpawnParameters spawnParameters;

	//spawn particules
	//if (ParticlesToSpawn != nullptr)
	//{
	//	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticlesToSpawn, playerLocation);
	//}

	//get the position and rotation of the respawn gameObject
	const FVector respawnLocation = respawnPosition->GetDefaultAttachComponent()->GetComponentLocation();
	const FRotator respawnRotation = respawnPosition->GetDefaultAttachComponent()->GetComponentRotation();
	//spawn new player
	AActor* newPlayer = GetWorld()->SpawnActor<AActor>(ActorToSpawn->GeneratedClass, respawnLocation, respawnRotation, spawnParameters);

	APawn* newPlayerPawn = Cast<APawn>(newPlayer);

	//assign controller to the player spawned
	player->Possess(newPlayerPawn);
	DefaultPawnClass = newPlayerPawn->GetClass();
}

void Acpp_projGameMode::SetRespawnPoint(AActor* respawnPoint)
{
	//set the respawn point
	respawnPosition = respawnPoint;
}

