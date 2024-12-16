// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAP_Plan.h"

UGOAP_Plan::UGOAP_Plan()
{
}

void UGOAP_Plan::Init(UGOAP_Goal* planGoal, TArray<UGOAP_Action*> planActions, int planCost)
{
	goal = planGoal;
	actions = planActions;
	cost = planCost;
}
