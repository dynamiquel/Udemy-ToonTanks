// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnTurret.h"
#include "Kismet/GameplayStatics.h"
#include "PawnTank.h"

// Called when the game starts or when spawned
void APawnTurret::BeginPlay()
{
    Super::BeginPlay();

    GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &APawnTurret::CheckFireCondition, FireRate, true);

    PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void APawnTurret::OnDestroy()
{
    Super::OnDestroy();
    Destroy();
}

// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!PlayerPawn || ReturnDistanceToPlayer() < FireRange)
        RotateTurret(PlayerPawn->GetActorLocation());
}

float APawnTurret::ReturnDistanceToPlayer() const
{
    if (!PlayerPawn)
        return .0f;

    return FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());
}

void APawnTurret::CheckFireCondition()
{
    if (!PlayerPawn || !PlayerPawn->GetIsPlayerAlive())
        return;

    if (ReturnDistanceToPlayer() <= FireRange)
        Fire();
}
