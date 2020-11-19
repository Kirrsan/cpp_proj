#pragma once
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "LevelStreamerActor.generated.h"

UCLASS()
class CPP_PROJ_API ALevelStreamerActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ALevelStreamerActor();

    // Called every frame
    virtual void Tick(float DeltaSeconds) override;

protected:

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UFUNCTION()
        void OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
        void OverlapEnds(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


    UPROPERTY(EditAnywhere)
        FName LevelToLoad;

    UPROPERTY(EditAnywhere)
        bool BlockMovementOnLoad;
private:
    // Overlap volume to trigger level streaming
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
        UBoxComponent* OverlapVolume;

};