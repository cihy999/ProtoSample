// Copyright Epic Games, Inc. All Rights Reserved.

// Self
#include "ProtoSampleGameMode.h"
// UE
#include "UObject/ConstructorHelpers.h"
// ProtoSample
#include "Message/TestMessage1.pb.h"
#include "Message/TestMessage2.pb.h"
#include "ProtoSampleCharacter.h"

void AProtoSampleGameMode::SerializeTestMessage1()
{
	ProtobufBlueprintSp::TestMessage1 Message;

	Message.set_val_1(50);
	Message.set_val_2(100);

	std::string BinaryData;
	if (Message.SerializeToString(&BinaryData))
	{
		SerializedBytes.Empty();
		SerializedBytes.Append(reinterpret_cast<const uint8*>(BinaryData.data()), BinaryData.size());

		UE_LOG(LogTemp, Log, TEXT("Serialized TestMessage1 to %d bytes"), SerializedBytes.Num());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to serialize TestMessage1"));
	}
}

void AProtoSampleGameMode::SerializeTestMessage2()
{
	ProtobufBlueprintSp::TestMessage2 Message;

	Message.set_val_1(500);

	std::string BinaryData;
	if (Message.SerializeToString(&BinaryData))
	{
		SerializedBytes.Empty();
		SerializedBytes.Append(reinterpret_cast<const uint8*>(BinaryData.data()), BinaryData.size());

		UE_LOG(LogTemp, Log, TEXT("Serialized TestMessage2 to %d bytes"), SerializedBytes.Num());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to serialize TestMessage1"));
	}
}

void AProtoSampleGameMode::DeserializeTestMessage1()
{
	ProtobufBlueprintSp::TestMessage1 Message;

	if (Message.ParseFromArray(SerializedBytes.GetData(), SerializedBytes.Num()))
	{
		UE_LOG(LogTemp, Log, TEXT("Deserialized TestMessage1 ==="));
		UE_LOG(LogTemp, Log, TEXT(" val_1 = %d"), Message.val_1());
		UE_LOG(LogTemp, Log, TEXT(" val_2 = %d"), Message.val_2());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to deserialize TestMessage1"));
	}
}

void AProtoSampleGameMode::DeserializeTestMessage2()
{
	ProtobufBlueprintSp::TestMessage2 Message;

	if (Message.ParseFromArray(SerializedBytes.GetData(), SerializedBytes.Num()))
	{
		UE_LOG(LogTemp, Log, TEXT("Deserialized TestMessage2 ==="));
		UE_LOG(LogTemp, Log, TEXT(" val_1 = %d"), Message.val_1());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to deserialize TestMessage2"));
	}
}

AProtoSampleGameMode::AProtoSampleGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}
