// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAP_Action.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/Strategies/AbstractStrategy.h"

UGOAP_Action::UGOAP_Action()
{
}

void UGOAP_Action::Start(UGOAP_Agent* agent)
{
	_strategy->Start(agent);
}

void UGOAP_Action::Update(float deltaTime)
{
	_strategy->Update(deltaTime);
}

void UGOAP_Action::Stop()
{
	_strategy->Stop();
}

bool UGOAP_Action::IsComplete()
{
	return _strategy->Complete();
}
