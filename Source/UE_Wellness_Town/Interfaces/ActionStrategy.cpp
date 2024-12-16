// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionStrategy.h"

// Add default functionality here for any IActionStrategy functions that are not pure virtual.

bool IActionStrategy::CanPerform()
{
    return false;
}

bool IActionStrategy::Complete()
{
    return false;
}

void IActionStrategy::Start(UGOAP_Agent* agent)
{
}

void IActionStrategy::Update(float deltaTime)
{
}

void IActionStrategy::Stop()
{
}
