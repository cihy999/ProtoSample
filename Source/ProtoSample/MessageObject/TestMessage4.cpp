// Fill out your copyright notice in the Description page of Project Settings.

// Self
#include "MessageObject/TestMessage4.h"

void UTestSubMessage2::InitMessage()
{
	Id = 3;
}

void UTestSubMessage1::InitMessage()
{
	Id = 2;

	if (!IsValid(SubMessage))
	{
		SubMessage = NewObject<UTestSubMessage2>(this);
		SubMessage->InitMessage();
	}
}

void UTestMessage4::InitMessage()
{
	Id = 1;

	if (!IsValid(SubMessage))
	{
		SubMessage = NewObject<UTestSubMessage1>(this);
		SubMessage->InitMessage();
	}
}
