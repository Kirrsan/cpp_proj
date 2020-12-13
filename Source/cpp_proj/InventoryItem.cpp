// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem.h"
#include "cpp_projCharacter.h"

// Sets default values for this component's properties
UInventoryItem::UInventoryItem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryItem::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryItem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor->ActorHasTag("Player")) {
        return;
    }

    Acpp_projCharacter* player = Cast<Acpp_projCharacter>(OtherActor);

    FItemStructure* item = itemsData->FindRow<FItemStructure>(itemRowName, "", true);

    if (item != nullptr) {
        if (player != nullptr) {
            player->AddItem(*item);
        }

        GetOwner()->Destroy();
    }

}

void UInventoryItem::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void UInventoryItem::AddCollisionDynamic()
{
    OverlapVolume->OnComponentBeginOverlap.AddDynamic(this, &UInventoryItem::OnBeginOverlap);
    OverlapVolume->OnComponentEndOverlap.AddDynamic(this, &UInventoryItem::OnEndOverlap);
}

