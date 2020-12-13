// Fill out your copyright notice in the Description page of Project Settings.


#include "MySaveInstance.h"
#include "cpp_projCharacter.h"
#include "MySaveGame.h"
#include "Kismet/GameplayStatics.h"

void UMySaveInstance::SaveGame(int slotID)
{
    UMySaveGame* SaveInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));

    if (SaveInstance == nullptr) {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Couldn't save"));
        return;
    }


    AActor* Avatar = Cast<AActor>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    Acpp_projCharacter* player = Cast<Acpp_projCharacter>(Avatar);

    if (player != nullptr) {
        playerHealth = player->health;

        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString::Printf(TEXT("Health player: %lld"), player->health));
        SaveInstance->saveHealth = playerHealth;
    }

    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString::Printf(TEXT("Health instance: %lld"), playerHealth));
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString::Printf(TEXT("Health save: %lld"), SaveInstance->saveHealth));

    if (UGameplayStatics::SaveGameToSlot(SaveInstance, "SaveGame" + slotID, 0)) {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Game saved in slot %lld"), slotID));
    }

}

void UMySaveInstance::LoadGame(int slotID)
{
    UMySaveGame* SaveInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot("SaveGame" + slotID, 0));

    if (SaveInstance == nullptr) {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No save available"));
        return;
    }

    isGameLoaded = true;

    playerHealth = SaveInstance->saveHealth;


    GEngine->AddOnScreenDebugMessage(-10, 3.f, FColor::Yellow, FString::Printf(TEXT("Health save: %lld"), SaveInstance->saveHealth));
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString::Printf(TEXT("Health instance: %d"), playerHealth));

}