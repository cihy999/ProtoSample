// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// UE
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
// 
#include "TestMessage4.generated.h"

UCLASS()
class PROTOSAMPLE_API UTestSubMessage2 : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	int32 Id = 0;

public:
	void InitMessage();
};

UCLASS()
class PROTOSAMPLE_API UTestSubMessage1 : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	int32 Id = 0;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UTestSubMessage2> SubMessage;

public:
	void InitMessage();
};

UCLASS()
class PROTOSAMPLE_API UTestMessage4 : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	int32 Id = 0;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UTestSubMessage1> SubMessage;

public:
	void InitMessage();
};
