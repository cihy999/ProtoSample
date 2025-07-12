// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

// UE
#include "CoreMinimal.h"
// ProtoSample
#include "GameFramework/GameModeBase.h"
//
#include "ProtoSampleGameMode.generated.h"

UCLASS(minimalapi)
class AProtoSampleGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SerializeTestMessage1();

	UFUNCTION(BlueprintCallable)
	void SerializeTestMessage2();

	UFUNCTION(BlueprintCallable)
	void DeserializeTestMessage1();

	UFUNCTION(BlueprintCallable)
	void DeserializeTestMessage2();

public:
	AProtoSampleGameMode();

private:
	TArray<uint8> SerializedBytes;
};
