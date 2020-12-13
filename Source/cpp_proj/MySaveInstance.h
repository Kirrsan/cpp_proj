// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MySaveInstance.generated.h"

/**
 * 
 */
UCLASS()
class CPP_PROJ_API UMySaveInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
		void SaveGame(int ID);
	UFUNCTION(BlueprintCallable)
		void LoadGame(int ID);


	FVector playerPos;
	int health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isGameLoaded = false;

};
