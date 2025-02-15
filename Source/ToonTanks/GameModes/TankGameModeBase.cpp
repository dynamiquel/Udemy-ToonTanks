// Fill out your copyright notice in the Description page of Project Settings.


#include "TankGameModeBase.h"
#include "ToonTanks/PawnTank.h"
#include "ToonTanks/PawnTurret.h"
#include "ToonTanks/PlayerControllers/PlayerControllerBase.h"
#include "Kismet/GameplayStatics.h"

void ATankGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    HandleGameStart();
}

void ATankGameModeBase::ActorDied(AActor* DeadActor)
{
    UE_LOG(LogTemp, Warning, TEXT("An Actor Died"));
    
    if (DeadActor == PlayerTank)
    {
        PlayerTank->OnDestroy();
        HandleGameOver(false);

        if (PlayerControllerRef)
            PlayerControllerRef->SetPlayerEnabledState(false);
    }
    else if (APawnTurret* DestroyedTurret = Cast<APawnTurret>(DeadActor))
    {
        DestroyedTurret->OnDestroy();

        if (--TargetTurrets <= 0)
            HandleGameOver(true);
    }
}

int32 ATankGameModeBase::GetTargetTurretCount() const
{
    TArray<AActor*> TurretActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawnTurret::StaticClass(), TurretActors);
    return TurretActors.Num();
}

void ATankGameModeBase::HandleGameStart()
{
    // Get Turret and Player pawn details.
    TargetTurrets = GetTargetTurretCount();
    PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
    PlayerControllerRef = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));
    
    GameStart();

    if (PlayerControllerRef)
    {
        PlayerControllerRef->SetPlayerEnabledState(false);
        
        FTimerHandle PlayerEnableHandle;
        FTimerDelegate PlayerEnableDelegate = FTimerDelegate::CreateUObject(PlayerControllerRef,
            &APlayerControllerBase::SetPlayerEnabledState, true);
        GetWorld()->GetTimerManager().SetTimer(PlayerEnableHandle, PlayerEnableDelegate, StartDelay, false);
    }
}

void ATankGameModeBase::HandleGameOver(bool PlayerWon)
{
    GameOver(PlayerWon);
}
