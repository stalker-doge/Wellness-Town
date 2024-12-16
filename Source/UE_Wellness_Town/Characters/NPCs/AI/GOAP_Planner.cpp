// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAP_Planner.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Belief.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Goal.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Action.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Node.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Agent.h"
#include "UE_Wellness_Town/Characters/NPCs/AI/GOAP_Plan.h"

UGOAP_Planner::UGOAP_Planner()
{
}

UGOAP_Plan* UGOAP_Planner::Plan(UGOAP_Agent* agent, TArray<UGOAP_Goal*> goals, UGOAP_Goal* lastGoal)
{
	TArray<UGOAP_Goal*> orderedGoals = goals.FilterByPredicate([&](const UGOAP_Goal* index) { return !(const_cast<UGOAP_Goal*>(index)->CheckIfFulfilled()); });

	if (lastGoal == nullptr)
	{
		orderedGoals.Sort([](const UGOAP_Goal& A, const UGOAP_Goal& B) { return &A._priority < &B._priority; });
	}
	else
	{
		orderedGoals.Sort([lastGoal](const UGOAP_Goal& A, const UGOAP_Goal& B) { return (&A == const_cast<UGOAP_Goal*>(lastGoal)) ? &A._priority < &B._priority - 1 : &A._priority < &B._priority; });
	}

	for (UGOAP_Goal* goal : orderedGoals)
	{
		UGOAP_Node* goalNode = NewObject<UGOAP_Node>();
		goalNode->Init(nullptr, nullptr, goal->_desiredEffects, 0);

		if (FindPath(goalNode, agent->_actions) == true)
		{
			if (goalNode->IsDead() == true)
			{
				continue;
			}

			TArray<UGOAP_Action*> actionStack;

			while (goalNode->neighbours.Num() > 0)
			{
				goalNode->neighbours.Sort([](const UGOAP_Node& a, const UGOAP_Node& b) { return a.cost < b.cost; });
				UGOAP_Node* cheapestNeighbour = goalNode->neighbours.Pop();

				goalNode = cheapestNeighbour;
				actionStack.Push(cheapestNeighbour->action);
			}

			UGOAP_Plan* plan = NewObject<UGOAP_Plan>();
			plan->Init(goal, actionStack, goalNode->cost);

			return plan;
		}
	}

	return nullptr;
}

bool UGOAP_Planner::FindPath(UGOAP_Node* parent, TArray<UGOAP_Action*> actions)
{
	actions.Sort([](const UGOAP_Action& a, const UGOAP_Action& b) { return a._cost < b._cost; });

	for (UGOAP_Action* action : actions)
	{
		TArray<UGOAP_Belief*> requiredEffects = parent->requiredEffects;

		requiredEffects.RemoveAll([&](const UGOAP_Belief* index) { return const_cast<UGOAP_Belief*>(index)->Evaluate(); });

		if (requiredEffects.Num() == 0)
		{
			return true;
		}

		if (action->_effects.Contains(*requiredEffects.CreateIterator()) == true)
		{
			TArray<UGOAP_Belief*>* newRequiredEffects = new TArray<UGOAP_Belief*>(requiredEffects);
			newRequiredEffects->RemoveAll([&](const UGOAP_Belief* index) { return action->_effects.Contains(index); });

			for (UGOAP_Belief* belief : action->_preconditions)
			{
				if (newRequiredEffects->Contains(belief) == false)
				{
					newRequiredEffects->Add(belief);
				}
			}

			TArray<UGOAP_Action*> newAvailableActions = TArray<UGOAP_Action*>(actions);
			newAvailableActions.Remove(action);

			UGOAP_Node* newNode = NewObject<UGOAP_Node>();
			newNode->Init(parent, action, *newRequiredEffects, parent->cost + action->_cost);

			if (FindPath(newNode, newAvailableActions) == true)
			{
				parent->neighbours.Add(newNode);
				newRequiredEffects->RemoveAll([&](const UGOAP_Belief* index) { return newNode->action->_preconditions.Contains(index); });
			}

			if (newRequiredEffects->Num() == 0)
			{
				delete newNode;
				delete newRequiredEffects;
				return true;
			}
		}
	}

	return false;
}
