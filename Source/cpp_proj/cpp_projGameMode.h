// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "cpp_projGameMode.generated.h"

UCLASS(minimalapi)
class Acpp_projGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	Acpp_projGameMode();
	void RespawnPlayer();
	void SetRespawnPoint(AActor* respawnPoint);

	//respawn AActor in the map, assigned via the method SetRespawnPoint() and a script on the respawn AActor.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Respawn)
		AActor* respawnPosition;

	//Player AActor to spawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Respawn)
		UBlueprint* ActorToSpawn;
	//particles AActor to spawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Respawn)
		AActor* ParticlesToSpawn;
private : 

};



