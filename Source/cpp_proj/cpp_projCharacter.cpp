// Copyright Epic Games, Inc. All Rights Reserved.
#include "cpp_projCharacter.h"
#include "DrawDebugHelpers.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "cpp_projGameMode.h"
#include "MySaveInstance.h"
#include "Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////
// Acpp_projCharacter

Acpp_projCharacter::Acpp_projCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	// declare overlap events
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &Acpp_projCharacter::OnBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &Acpp_projCharacter::OnEndOverlap);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;


	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//get the path to the bullets blueprint in the project
	FStringAssetReference itemRef = "Blueprint'/Game/ThirdPersonCPP/Blueprints/BulletActor1_Blueprint.BulletActor1_Blueprint'";
	UObject* itemObj = itemRef.ResolveObject();
	ActorToSpawn = Cast<UBlueprint>(itemObj);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}


// Called when the game starts or when spawned
void Acpp_projCharacter::BeginPlay()
{
	Super::BeginPlay();
	UMySaveInstance* gameInstance = Cast<UMySaveInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (gameInstance != nullptr) {
		health = gameInstance->playerMaxHealth;
		if (gameInstance->isGameLoaded) {
			health = gameInstance->playerHealth;
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString::Printf(TEXT("Health player : %d"), health));
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString::Printf(TEXT("Health save : %d"), gameInstance->playerHealth));
		}
	}
}


//////////////////////////////////////////////////////////////////////////
// Tick



void Acpp_projCharacter::Tick(float DeltaSeconds)
{
	//if linetrace can be casted
	if (isInteracting)
	{
		LineTracePickUp();
	}
	else
	{
		//if linetrace needs to stop and we have an object held
		LineTraceDrop();
	}

	if (isShooting && !hasShot)
	{
		//if we want to shoot and the fireRate timer is done
		Shoot();
	}
	else if (hasShot)
	{
		//fire rate timer
		FireRateTimer += DeltaSeconds;
		if (FireRateTimer >= FireRate)
		{
			FireRateTimer = 0;
			hasShot = false;
		}
	}
	
}

void Acpp_projCharacter::LineTracePickUp()
{
	//move linetrace with player


	//get linetrace direction
	FVector worldPosition = HeldObjectsPositionActor->GetComponentLocation();
	FVector forwardVector = HeldObjectsPositionActor->GetForwardVector();

	FVector lastPosition = GetCharacterMovement()->GetLastUpdateLocation() * forwardVector * 0.1f + worldPosition;

	FVector endVector = forwardVector * LineTraceDistance + worldPosition;

	//if we're not holding an object, do a linetrace
	if (!isHoldingObject)
	{
		//lineTrace debug
		const FName TraceTag("MyTraceTag");
		GetWorld()->DebugDrawTraceTag = TraceTag;
		FCollisionQueryParams CollisionParams;
		CollisionParams.TraceTag = TraceTag;

		//linetrace
		FHitResult ObjectHitByLineTrace;
		bool hasHitSomething = GetWorld()->LineTraceSingleByChannel(ObjectHitByLineTrace, lastPosition, endVector, ECollisionChannel::ECC_Visibility, TraceTag);

		//if hit actor can be picked up
		if (hasHitSomething && ObjectHitByLineTrace.GetComponent()->Mobility == EComponentMobility::Movable && ObjectHitByLineTrace.GetActor()->ActorHasTag("Holdable"))
		{
			//pick up object and stop the raycast
			currentObjectHeld = ObjectHitByLineTrace.GetComponent();
			currentObjectHeld->SetSimulatePhysics(false);
			AActor* objectHeldOwner = currentObjectHeld->GetOwner();
			objectHeldOwner->AttachToComponent(HeldObjectsPositionActor, FAttachmentTransformRules::SnapToTargetIncludingScale);
			isHoldingObject = true;
		}
	}
	else
	{
		//refresh object held position without casting linetrace
		AActor* objectHeldOwner = currentObjectHeld->GetOwner();
		objectHeldOwner->AttachToComponent(HeldObjectsPositionActor, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}

}

void Acpp_projCharacter::LineTraceDrop()
{
	//drop object held
	if (IsValid(currentObjectHeld))
	{
		currentObjectHeld->SetSimulatePhysics(true);
		AActor* objectHeldOwner = currentObjectHeld->GetOwner();
		objectHeldOwner->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		currentObjectHeld = nullptr;
		isHoldingObject = false;
	}
}

void Acpp_projCharacter::Shoot()
{
	//spawn bullet
	const FVector respawnLocation = ShootPositionActor->GetComponentLocation();
	const FRotator respawnRotation = ShootPositionActor->GetComponentRotation();
	const FActorSpawnParameters spawnParameters;

	AActor* bullet = GetWorld()->SpawnActor<AActor>(ActorToSpawn->GeneratedClass, respawnLocation, respawnRotation, spawnParameters);
	hasShot = true;
}

void Acpp_projCharacter::StopShooting()
{
	isShooting = false;
}

void Acpp_projCharacter::Shooting()
{
	isShooting = true;
}



//////////////////////////////////////////////////////////////////////////
// Input

void Acpp_projCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//binded to "left control"
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &Acpp_projCharacter::Crouching);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &Acpp_projCharacter::UnCrouching);

	//binded to "E"
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &Acpp_projCharacter::Interact);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &Acpp_projCharacter::UnInteract);

	//binded to left mouse button
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &Acpp_projCharacter::Shooting);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &Acpp_projCharacter::StopShooting);

	//binded to shiftAcpp_projCharacter
	PlayerInputComponent->BindAction("Strafing", IE_Pressed, this, &Acpp_projCharacter::ActivateStrafe);
	PlayerInputComponent->BindAction("Strafing", IE_Released, this, &Acpp_projCharacter::DeactivateSrafe);


	PlayerInputComponent->BindAxis("MoveForward", this, &Acpp_projCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &Acpp_projCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &Acpp_projCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &Acpp_projCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &Acpp_projCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &Acpp_projCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &Acpp_projCharacter::OnResetVR);
}

void Acpp_projCharacter::ActivateStrafe()
{
	isStrafing = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void Acpp_projCharacter::DeactivateSrafe()
{
	isStrafing = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void Acpp_projCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		//directionValue & isGoingSide are use for animation transition
		directionValue = Value;
		isGoingSide = false;

		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void Acpp_projCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		directionValue = Value;
		isGoingSide = true;

		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


void Acpp_projCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void Acpp_projCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}



void Acpp_projCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void Acpp_projCharacter::Crouching()
{
	//end crouch
	Crouch();
}

void Acpp_projCharacter::UnCrouching()
{
	//start crouch
	UnCrouch();
}

void Acpp_projCharacter::Interact()
{
	//start raycast to pick up objects
	isInteracting = true;
}

void Acpp_projCharacter::UnInteract()
{
	//end raycast
	isInteracting = false;
}


void Acpp_projCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void Acpp_projCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void Acpp_projCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//respawn if hit by lava
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("OVERLAP")));
	if (OtherActor->ActorHasTag("Lava"))
	{
		Acpp_projGameMode* gameMode = (Acpp_projGameMode*)GetWorld()->GetAuthGameMode();
		gameMode->RespawnPlayer();
	}
}

void Acpp_projCharacter::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void Acpp_projCharacter::AddItem(FItemStructure itemToAdd)
{
	int index = 0;
	if (inventory.Find(itemToAdd, index))
	{
		inventoryTracking[index] += 1;
	}
	else {
		inventory.Add(itemToAdd);
		inventoryTracking.Add(itemToAdd.numberItemsInItem);
	}
}

void Acpp_projCharacter::SellItem(int itemToSellIndex)
{
	money += inventory[itemToSellIndex].itemValue;
	RemoveItemFromInventory(itemToSellIndex);
}

void Acpp_projCharacter::UseItem(int itemToUseIndex)
{

	/*UMyGameInstance* gameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	health += inventory[itemToUseIndex].impactOnPlayerLife;
	if (gameInstance != nullptr) {
		if (health > gameInstance->playerMaxHealth) {
			health = gameInstance->playerMaxHealth;
		}
	}*/
	RemoveItemFromInventory(itemToUseIndex);
}

void Acpp_projCharacter::RemoveItemFromInventory(int itemIndex)
{
	inventoryTracking[itemIndex] -= 1;
	if (inventoryTracking[itemIndex] <= 0) {
		//inventory.RemoveAt(itemIndex);
		inventoryTracking.RemoveAt(itemIndex);
	}
	OnItemOut();
}



void Acpp_projCharacter::Destroy()
{
	Super::Destroyed();
}


