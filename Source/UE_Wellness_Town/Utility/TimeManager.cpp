// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeManager.h"

void UTimeManager::Update(float deltaTime)
{
	timer += deltaTime;

	if (timer >= 24)
	{
		timer = 0;
	}
}

int UTimeManager::GetHours()
{
	return timer;
}
