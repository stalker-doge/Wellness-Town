// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAP_Node.h"

UGOAP_Node::UGOAP_Node()
{
}

void UGOAP_Node::Init(UGOAP_Node* nodeParent, UGOAP_Action* nodeAction, TArray<UGOAP_Belief*> nodeEffects, int nodeCost)
{
	parent = nodeParent;
	action = nodeAction;
	requiredEffects = nodeEffects;
	cost = nodeCost;
}

bool UGOAP_Node::IsDead()
{
	return (neighbours.Num() == 0 && action == nullptr);
}
