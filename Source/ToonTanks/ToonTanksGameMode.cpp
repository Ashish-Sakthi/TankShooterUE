// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"
#include "Kismet/GameplayStatics.h"


void AToonTanksGameMode::BeginPlay()
{
	Super::BeginPlay();
	HandleGameStart();
}

void AToonTanksGameMode::HandleGameStart()
{
	TargetTowers = GetTargetTowersCount();
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	PlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	
	StartGame(); 

	if (PlayerController)
	{
		PlayerController->SetPlayerEnabledState(false);
		
		FTimerHandle PlayerEnableTimerHandle;
		
		FTimerDelegate	TimerDelegate = FTimerDelegate::CreateUObject( // using to pass a function will input parameters
			PlayerController, 
			&AToonTanksPlayerController::SetPlayerEnabledState,
			true // input parameter of the function 
		);

		GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle, TimerDelegate, StartDelay, false);
	}
}

void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{
	if (DeadActor == Tank)
	{
		Tank->HandleDestruction();
		if(PlayerController)
		{
			PlayerController->SetPlayerEnabledState(false);
		}

		GameOver(false);
	}

	else if (ATower* DestoryedTower = Cast<ATower>(DeadActor))//If dead actor is ATower Class the cast will be successful
	{																//and go in . else it will not go in
		DestoryedTower->HandleDestruction();
		--TargetTowers;
		if (TargetTowers == 0)
		{
			GameOver(true);
		}
	}
}

int32 AToonTanksGameMode::GetTargetTowersCount()
{
	TArray<AActor*> Towers;
	UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);
	return Towers.Num();
}