// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAP_Belief.h"

UGOAP_Belief::UGOAP_Belief()
{
}

bool UGOAP_Belief::Evaluate()
{
    return _condition.Execute();
}

FString UGOAP_Belief::GetName()
{
    if (_name.IsEmpty() == true)
    {
        return TEXT("NO NAME");
    }

    return _name;
}
