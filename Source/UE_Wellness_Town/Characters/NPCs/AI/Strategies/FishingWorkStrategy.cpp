// Fill out your copyright notice in the Description page of Project Settings.


#include "FishingWorkStrategy.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Agent.h"
#include "UE_Wellness_Town/Characters/NPCs/NPC_Base.h"
#include "UE_Wellness_Town/Objects/FishingRod.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SplineComponent.h"

void UFishingWorkStrategy::Start(UGOAP_Agent* agent)
{
    _agent = agent;
    _npc = _agent->_owner;

    if (_fishingRod == nullptr)
    {
        FActorSpawnParameters spawnParams;
        spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        _fishingRod = _npc->GetWorld()->SpawnActor<AFishingRod>(_agent->GetFishingRodDefault(), _npc->GetActorLocation(), FRotator::ZeroRotator, spawnParams);
    }

	_fishingRod->GetStaticMesh()->SetVisibility(true);
	_npc->PickUp(_fishingRod);

	if (_splineComponent == nullptr)
	{
		_splineComponent = _npc->GetSplineComponent();
	}
}

void UFishingWorkStrategy::Update(float deltaTime)
{
	if (_npc->GetMesh()->IsVisible() == false)
	{
		return;
	}

	if (_fishingRod->IsFishing() == true)
	{
		return;
	}

	_actionTimer += deltaTime;

	if (_actionTimer > 5)
	{
		CastRod();
		_actionTimer = 0;
	}
}

void UFishingWorkStrategy::Stop()
{
	_npc->DropItem();
	_fishingRod->GetStaticMesh()->SetVisibility(false);
	_fishingRod->DisableCollision();

	_agent->SetIsAtWorkPlace(false);
}

bool UFishingWorkStrategy::Complete()
{
	return (!_agent->_isWorkHours && _fishingRod->IsFishing() == false);
}

void UFishingWorkStrategy::CreateSpline()
{
	_splineComponent->ClearSplinePoints();

	FVector unitDirection = _npc->GetActorForwardVector() + _npc->GetActorUpVector();
	unitDirection.Normalize();

	FPredictProjectilePathParams params;
	params.StartLocation = _npc->GetActorLocation();
	params.LaunchVelocity = unitDirection * 1000;
	params.MaxSimTime = 3;
	params.DrawDebugType = EDrawDebugTrace::ForDuration;
	params.DrawDebugTime = 1;

	FPredictProjectilePathResult result;
	UGameplayStatics::PredictProjectilePath(_fishingRod, params, result);

	for (int i = 0; i < result.PathData.Num(); i++)
	{
		_splineComponent->AddSplinePoint(result.PathData[i].Location, ESplineCoordinateSpace::World, true);
	}

	_splineComponent->UpdateSpline();
}

void UFishingWorkStrategy::CastRod()
{
	CreateSpline();
	_npc->UseItem(_splineComponent);
}
