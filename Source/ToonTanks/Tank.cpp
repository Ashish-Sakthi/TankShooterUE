// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"

ATank::ATank()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	TankPlayerController = Cast<APlayerController>(GetController());
	
	if (TankPlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(TankPlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(TankIMC, 0);
		}
	}
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TankPlayerController)
	{
		FHitResult HitResult;
		TankPlayerController->GetHitResultUnderCursor(
			ECollisionChannel::ECC_Visibility, 
			true, 
			HitResult);

		FVector ImpactPoint = HitResult.ImpactPoint;
		//DrawDebugSphere(GetWorld(), ImpactPoint,25,12,FColor::Blue,false);
		RotateTurret(ImpactPoint);
	}
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();
	bAlive = false;
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedPlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedPlayerInput->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ATank::Move);
		EnhancedPlayerInput->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ATank::Turn);
		EnhancedPlayerInput->BindAction(FireAction, ETriggerEvent::Started , this, &ABasePawn::Fire);
	}
}

void ATank::Move(const FInputActionValue& Value)
{
	const float value = Value.Get<float>();
	FVector DeltaLocation = FVector::ZeroVector;
	DeltaLocation.X = value * speed * UGameplayStatics::GetWorldDeltaSeconds(this) ;
	AddActorLocalOffset(DeltaLocation);
}

void ATank::Turn(const FInputActionValue& Value)
{
	const float value = Value.Get<float>();
	FRotator DeltaRotation = FRotator::ZeroRotator;
	DeltaRotation.Yaw = value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorWorldRotation(DeltaRotation);
}