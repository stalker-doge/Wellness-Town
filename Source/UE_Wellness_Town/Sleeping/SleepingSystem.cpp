// Fill out your copyright notice in the Description page of Project Settings.


#include "SleepingSystem.h"

SleepingSystem::SleepingSystem()
{
	//initialize the current day to 1
	CurrentDay = 1;
	//initialize the current time to day
	CurrentTime = TimeOfDay::Day;
}

SleepingSystem::~SleepingSystem()
{
}

void SleepingSystem::SetCurrentDay(int NewDay)
{
	CurrentDay = NewDay;
}

int SleepingSystem::GetCurrentDay()
{
	return CurrentDay;
}

void SleepingSystem::Sleep()
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
