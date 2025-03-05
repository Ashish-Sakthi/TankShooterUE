// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Projectile.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Sound/SoundBase.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spwan Point"));
	SpawnPoint->SetupAttachment(TurretMesh);	

}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0,ToTarget.Rotation().Yaw, 0);
	TurretMesh->SetWorldRotation(FMath::RInterpTo(
		TurretMesh->GetComponentRotation(), 
		LookAtRotation,
		UGameplayStatics::GetWorldDeltaSeconds(this),
		5
	));
}

void ABasePawn::Fire()
{
	FVector ProjectileSpawLocation = SpawnPoint->GetComponentLocation();
	
	/*DrawDebugSphere(
		GetWorld(),
		ProjectileSpawLocation,
		25, 12,
		FColor::Red,
		false,
		5
	);*/

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileClass, 
		SpawnPoint->GetComponentLocation(), 
		SpawnPoint->GetComponentRotation()
	);
	Projectile->SetOwner(this);
}

void ABasePawn::HandleDestruction()
{
	//To-Do Visual & sound effects
	if (DestroyParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			this, DestroyParticles, 
			GetActorLocation(),
			GetActorRotation()
		);
	}
	if(DestroySound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DestroySound, GetActorLocation());
	}
	if (DestroyCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DestroyCameraShakeClass);
	}
	
}