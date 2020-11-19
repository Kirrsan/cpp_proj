// Fill out your copyright notice in the Description page of Project Settings.


#include "RespawnComponent.h"
#include "D:\UnrealProjects\TP_cpp\cpp_proj\Source\cpp_proj\cpp_projGameMode.h"

// Sets default values for this component's properties
URespawnComponent::URespawnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URespawnComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	AGameModeBase* GM = GetWorld()->GetAuthGameMode();
	Acpp_projGameMode* GameMode = Cast<Acpp_projGameMode>(GM);
	GameMode->SetRespawnPoint(this->GetOwner());
}


// Called every frame
void URespawnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

