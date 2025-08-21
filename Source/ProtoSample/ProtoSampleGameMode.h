// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

// UE
#include "CoreMinimal.h"
// Protobuf
#include "google/protobuf/message.h"
// ProtoSample
#include "GameFramework/GameModeBase.h"
#include "Message/TestMessage4.pb.h"
//
#include "ProtoSampleGameMode.generated.h"

class UTestMessage3;
class UTestMessage4;

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

	UFUNCTION(BlueprintCallable)
	void CreateTestMessage3();

	UFUNCTION(BlueprintCallable)
	void CreateTestMessage4();

public:
	AProtoSampleGameMode();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UTestMessage3> TestMessage3_Class;

	UPROPERTY(Transient, BlueprintReadOnly)
	TObjectPtr<UTestMessage3> TestMessage3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UTestMessage4> TestMessage4_Class;

	UPROPERTY(Transient, BlueprintReadOnly)
	TObjectPtr<UTestMessage4> TestMessage4;

	TUniquePtr<protobuf::TestMessage4> ProtoTestMessage4;

private:
	TArray<uint8> SerializedBytes;

	void DumpMessageProperty(UObject* Message);
	void DumpObjectProperty(UObject* Message);
	void DumpMessageField(google::protobuf::Message& Message);
	void DumpObjectField(google::protobuf::Message& Message);
};
