// Fill out your copyright notice in the Description page of Project Settings.


#include "SleepingSystem.h"

void USleepingSystem::BeginPlay()
{
	Super::BeginPlay();
}

USleepingSystem::USleepingSystem()
{
	//initialize the current day to 1
	CurrentDay = 1;
	//initialize the current time to day
	CurrentTime = TimeOfDay::Day;
}

USleepingSystem::~USleepingSystem()
{
}

void USleepingSystem::SetCurrentDay(int NewDay)
{
	CurrentDay = NewDay;
}

int USleepingSystem::GetCurrentDay()
{
	return CurrentDay;
}

void USleepingSystem::Sleep()
{
	//if it is day time, set time to night
	if (CurrentTime == TimeOfDay::Day)
	{
		CurrentTime = TimeOfDay::Night;
	}
	//if it is night time, set time to day
	else if (CurrentTime == TimeOfDay::Night)
	{
		CurrentTime = TimeOfDay::Day;
		CurrentDay++;
	}
}
