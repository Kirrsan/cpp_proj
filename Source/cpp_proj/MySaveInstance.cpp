// Fill out your copyright notice in the Description page of Project Settings.


#include "MySaveInstance.h"
#include "cpp_projCharacter.h"
#include "MySaveGame.h"
#include "Kismet/GameplayStatics.h"

void UMySaveInstance::SaveGame(int ID)
{
    UMySaveGame* mySave = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));

    if (mySave == nullptr) {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Did not save"));
        return;
    }


    AActor* playerActor = Cast<AActor>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    Acpp_projCharacter* player = Cast<Acpp_projCharacter>(playerActor);

    if (player != nullptr) {
        playerPos = player->GetActorLocation();
        health = player->health;

        mySave->PlayerPos = playerPos;
        mySave->HealthSaved = health;
    }

    UGameplayStatics::SaveGameToSlot(mySave, "SaveGame" + ID, 0);

    if (UGameplayStatics::SaveGameToSlot(mySave, "SaveGame" + ID, 0)) {
    }

}

void UMySaveInstance::LoadGame(int ID)
{
    UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
    SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot("SaveGame" + ID, 0));

    if (SaveGameInstance == nullptr) {
        return;
    }
    playerPos = SaveGameInstance->PlayerPos;
    health = SaveGameInstance->HealthSaved;

    isGameLoaded = true;
}