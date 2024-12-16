// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAP_Action.h"

UGOAP_Action::UGOAP_Action()
{
}

void UGOAP_Action::Start()
{
	_strategy->Start();
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
