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

	// 確認未知欄位
	const google::protobuf::Reflection* Reflection = Message.GetReflection();
	if (Reflection)
	{
		const google::protobuf::UnknownFieldSet& UnknownFields = Reflection->GetUnknownFields(Message);

		for (int i = 0; i < UnknownFields.field_count(); ++i)
		{
			const auto& Field = UnknownFields.field(i);
			int FieldNumber = Field.number();

			switch (Field.type())
			{
			case google::protobuf::UnknownField::TYPE_VARINT:
				UE_LOG(LogTemp, Log, TEXT(" [Unknown] Field %d: VARINT = %lld"), FieldNumber, Field.varint());
				break;
			case google::protobuf::UnknownField::TYPE_FIXED32:
				UE_LOG(LogTemp, Log, TEXT(" [Unknown] Field %d: FIXED32 = %u"), FieldNumber, Field.fixed32());
				break;
			case google::protobuf::UnknownField::TYPE_FIXED64:
				UE_LOG(LogTemp, Log, TEXT(" [Unknown] Field %d: FIXED64 = %llu"), FieldNumber, Field.fixed64());
				break;
			case google::protobuf::UnknownField::TYPE_LENGTH_DELIMITED:
				UE_LOG(LogTemp, Log, TEXT(" [Unknown] Field %d: BYTES size = %d"), FieldNumber, Field.length_delimited().size());
				break;
			default:
				UE_LOG(LogTemp, Warning, TEXT(" [Unknown] Field %d: Unsupported type"), FieldNumber);
				break;
			}
		}
	}
}

AProtoSampleGameMode::AProtoSampleGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}
