// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/DataTable.h"
#include "InventoryItem.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPP_PROJ_API UInventoryItem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryItem();


public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    //UPROPERTY(BlueprintReadWrite, Category = "Properties")
    //FString actorName = "";
    //
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
        FName itemRowName = "0";
    //
    //UPROPERTY(BlueprintReadWrite, Category = "Properties")
    //FColor actorColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
        USphereComponent* OverlapVolume;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        UDataTable* itemsData;

    UFUNCTION(BlueprintCallable)
        void AddCollisionDynamic();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    UFUNCTION()
        void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
        void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};

USTRUCT(BlueprintType)
struct FItemStructure : public FTableRowBase
{
    GENERATED_USTRUCT_BODY();


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
        FString itemName = "";

    bool operator==(const FItemStructure& OtherItem) const
    {
        return itemName == OtherItem.itemName;
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
        int itemValue = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
        UTexture* itemTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
        int numberItemsInItem = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
        int impactOnPlayerLife = 0;
};


