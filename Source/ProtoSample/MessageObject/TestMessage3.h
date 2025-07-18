// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// UE
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
// 
#include "TestMessage3.generated.h"

UCLASS(Blueprintable)
class PROTOSAMPLE_API UTestMessage3 : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	bool BoolVal = false;

	UPROPERTY(BlueprintReadWrite)
	int32 Int32Val = 0;
};
