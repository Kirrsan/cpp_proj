// Fill out your copyright notice in the Description page of Project Settings.
#include "Components/DecalComponent.h"
#include "Engine/DecalActor.h"
#include "BulletActor.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ABulletActor::ABulletActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    OverlapVolume = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapVolume"));
    RootComponent = OverlapVolume;


}

// Called when the game starts or when spawned
void ABulletActor::BeginPlay()
{
	Super::BeginPlay();
    OverlapVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &ABulletActor::OnBeginOverlap);
}

// Called every frame
void ABulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABulletActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->ActorHasTag("Player") || OtherActor->ActorHasTag("Bullet")) return;
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap Begin"));
    FVector spawnLocation = SweepResult.Location;
    SpawnDecal(spawnLocation, OtherComp, SweepResult.Normal.Rotation());
}

void ABulletActor::SpawnDecal(FVector spawnLocation, UPrimitiveComponent* OtherComp, FRotator rotation)
{
    UGameplayStatics::SpawnDecalAttached(decalToSpawn, FVector(32.0f, 64.0f, 64.0f), OtherComp, "name", spawnLocation, rotation, EAttachLocation::KeepWorldPosition, 60);
    Super::Destroy();
}