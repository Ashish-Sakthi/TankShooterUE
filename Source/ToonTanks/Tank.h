// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "InputActionValue.h"
#include "Tank.generated.h"

class UInputMappingContext;
class UInputAction;
/**
 * 
 */
UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	ATank();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HandleDestruction();

	APlayerController* GetTankPlayerController() const { return TankPlayerController; }

	bool bAlive = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* TankIMC;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* TurnAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* FireAction;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "True"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "True"))
	class UCameraComponent* CameraComp;
	
	void Move(const FInputActionValue& Value);
	void Turn(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, Category = "Movement")
	float speed = 500;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnRate = 60;

	APlayerController* TankPlayerController;
};
