// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAP_Goal.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Belief.h"

UGOAP_Goal::UGOAP_Goal()
{
}

bool UGOAP_Goal::CheckIfFulfilled()
{
	if (_desiredEffects.Num() == 0)
	{
		return true;
	}

	for (UGOAP_Belief* belief : _desiredEffects)
	{
		if (belief->Evaluate() == false)
		{
			return false;
		}
	}

	return true;
}
