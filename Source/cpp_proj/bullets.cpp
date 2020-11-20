// Fill out your copyright notice in the Description page of Project Settings.
#include "Components/DecalComponent.h"
#include "Engine/DecalActor.h"
#include "bullets.h"

// Sets default values for this component's properties
Ubullets::Ubullets()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...


}


// Called when the game starts
void Ubullets::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void Ubullets::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    SpawnDecal(GetOwner()->GetActorLocation());
}

void Ubullets::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}


// Called every frame
void Ubullets::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void Ubullets::SpawnDecal(FVector spawnLocation)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap Begin"));
    ADecalActor* decal = GetWorld()->SpawnActor<ADecalActor>(spawnLocation, FRotator(0, 0, 0));
    if (decal)
    {
        decal->SetDecalMaterial(decalToSpawn);
        decal->SetLifeSpan(60.0f);
        decal->GetDecal()->DecalSize = FVector(32.0f, 64.0f, 64.0f);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No decal spawned"));
    }
}