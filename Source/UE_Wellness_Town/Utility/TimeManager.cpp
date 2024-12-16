// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeManager.h"

void UTimeManager::Update(float deltaTime)
{
	timer += deltaTime;
	UpdateHoursAndMins();
}

void UTimeManager::UpdateHoursAndMins()
{
	if (timer >= 24)
	{
		timer = 0;
	}

	int h = static_cast<int>(timer);
	int m = static_cast<int>((timer - h) * 60);

	hours = h;
	minutes = m;

	GEngine->AddOnScreenDebugMessage(9, 5, FColor::Yellow, FString::Printf(TEXT("%d: %d"), hours, minutes));
}

int UTimeManager::GetHours()
{
	return hours;
}

int UTimeManager::GetMinutes()
{
	return minutes;
}
