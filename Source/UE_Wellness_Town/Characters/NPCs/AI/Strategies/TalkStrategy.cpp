// Fill out your copyright notice in the Description page of Project Settings.


#include "TalkStrategy.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Agent.h"

void UTalkStrategy::Start(UGOAP_Agent* agent)
{
	_timer = 0;

	_agent = agent;
	//_agent->_canTalk = false;

	if (_dialogueTarget != nullptr)
	{
		//_dialogueTarget->_canTalk = false;
	}
}

void UTalkStrategy::Update(float deltaTime)
{
	_timer += deltaTime;

	if (_timer >= 5)
	{
		//_agent->_canTalk = true;

		if (_dialogueTarget != nullptr)
		{
			//_dialogueTarget->_canTalk = true;
		}
	}
}

void UTalkStrategy::Stop()
{
	//_agent->_talkingTo = nullptr;
}

bool UTalkStrategy::Complete()
{
	return true;
	//return _agent->_canTalk == true;
}
