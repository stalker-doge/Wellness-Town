// Fill out your copyright notice in the Description page of Project Settings.


#include "BugCatcherWorkStrategy.h"
#include "Components/SphereComponent.h"
#include "UE_Wellness_Town/Characters/Animals/Bug.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/Sensors/GOAP_BugSensor.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Agent.h"
#include "UE_Wellness_Town/Characters/NPCs/NPC_Base.h"
#include "UE_Wellness_Town/Objects/BugNet.h"
void UBugCatcherWorkStrategy::Start(UGOAP_Agent* agent)
{
	_agent = agent;
	_npc = _agent->_owner;

	if (_bugNet == nullptr)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		_bugNet = _npc->GetWorld()->SpawnActor<ABugNet>(_agent->GetBugNetDefault(), _npc->GetActorLocation(), FRotator::ZeroRotator, spawnParams);
	}

	_bugNet->GetStaticMesh()->SetVisibility(true);
	_npc->PickUp(_bugNet);

	if (_sensor == nullptr)
	{
		CreateSensor();
	}

	_sensor->SetIsActive(true);
}

void UBugCatcherWorkStrategy::Update(float deltaTime)
{
	_timer += deltaTime;

	if (_timer < 0.2)
	{
		return;
	}

	_timer = 0;

	if (_target != nullptr)
	{
		_agent->SetDestination(_target->GetActorLocation());

		if (IsInTargetRange() == true)
		{
			_agent->_owner->UseItem(nullptr);
			_sensor->UpdateClosestBug();
			_target = nullptr;
		}

		return;
	}

	_target = _sensor->GetClosestBug();
}

void UBugCatcherWorkStrategy::Stop()
{
	_sensor->SetIsActive(false);

	_npc->DropItem();
	_bugNet->GetStaticMesh()->SetVisibility(false);
	_bugNet->DisableCollision();

	_agent->SetDestination(_agent->GetActorLocation());
	_agent->SetIsAtWorkPlace(false);
}

bool UBugCatcherWorkStrategy::Complete()
{
	return !_agent->_isWorkHours;
}

void UBugCatcherWorkStrategy::CreateSensor()
{
	_sensor = NewObject<UGOAP_BugSensor>(_agent->GetOwner(), TEXT("Bug Sensor"));
	checkf(_sensor, TEXT("Failure to Create Bug Sensor"));

	_sensor->RegisterComponent();
	_sensor->Init(5000, _agent->GetOwner());
}

bool UBugCatcherWorkStrategy::IsInTargetRange()
{
	if (_target == nullptr)
	{
		return false;
	}

	float distance = (_agent->GetActorLocation() - _target->GetActorLocation()).Length();

	if (distance < 500)
	{
		return true;
	}

	return false;
}

