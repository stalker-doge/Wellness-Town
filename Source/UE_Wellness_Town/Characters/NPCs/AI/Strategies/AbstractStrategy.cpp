// Fill out your copyright notice in the Description page of Project Settings.


#include "AbstractStrategy.h"

bool UAbstractStrategy::CanPerform()
{
	return false;
}

bool UAbstractStrategy::Complete()
{
	return false;
}

void UAbstractStrategy::Start(UGOAP_Agent* agent)
{
}

void UAbstractStrategy::Update(float deltaTime)
{
}

void UAbstractStrategy::Stop()
{
}
