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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		AActor* respawnPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UBlueprint* ActorToSpawn;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		AActor* ParticlesToSpawn;
private : 

};



