// Copyright Epic Games, Inc. All Rights Reserved.

// Self
#include "ProtoSampleGameMode.h"
// UE
#include "UObject/ConstructorHelpers.h"
// ProtoSample
#include "Message/TestMessage1.pb.h"
#include "Message/TestMessage2.pb.h"
#include "MessageObject/TestMessage3.h"
#include "ProtoSampleCharacter.h"

void AProtoSampleGameMode::SerializeTestMessage1()
{
	protobuf::TestMessage1 Message;

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
	protobuf::TestMessage2 Message;

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
	protobuf::TestMessage1 Message;

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
	protobuf::TestMessage2 Message;

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

void AProtoSampleGameMode::CreateTestMessage3()
{
	if (TestMessage3Class)
	{
		TestMessage3 = NewObject<UTestMessage3>(this, TestMessage3Class.Get());
		TestMessage3->NativeMessage = MakeUnique<protobuf::TestMessage3>();

		UE_LOG(LogTemp, Log, TEXT("Create %s"), *TestMessage3->GetName());
		DumpMessageProperty(TestMessage3);

		const google::protobuf::Descriptor* Descriptor = TestMessage3->NativeMessage->GetDescriptor();
		const google::protobuf::Reflection* Reflection = TestMessage3->NativeMessage->GetReflection();

		UE_LOG(LogTemp, Log, TEXT(" %s's field ==="), UTF8_TO_TCHAR(Descriptor->name().c_str()));

		int32 FieldCount = Descriptor->field_count();
		for (int32 i = 0; i < FieldCount; i++)
		{
			const google::protobuf::FieldDescriptor* Field = Descriptor->field(i);

			FString Desc = FString::Printf(TEXT(" (%s)%s: "),
				UTF8_TO_TCHAR(Field->cpp_type_name()),
				UTF8_TO_TCHAR(Field->name().c_str())
			);

			UE_LOG(LogTemp, Log, TEXT(" %s"), *Desc);
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

void AProtoSampleGameMode::DumpMessageProperty(UObject* Message)
{
	if (!IsValid(Message))
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT(" %s's property ==="), *Message->GetName());

	for (TFieldIterator<FProperty> PropertyIt(Message->GetClass()); PropertyIt; ++PropertyIt)
	{
		FProperty* Property = *PropertyIt;
		if (Property)
		{
			FString TypeName = Property->GetCPPType();
			FString PropertyName = Property->GetName();
			FString ValueString = "";

			if (FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property))
			{
				bool V = BoolProperty->GetPropertyValue_InContainer(Message);
				ValueString = FString::Printf(TEXT("%s"), V ? TEXT("true") : TEXT("false"));
			}
			else if (FInt8Property* Int8Property = CastField<FInt8Property>(Property))
			{
				int8 V = Int8Property->GetPropertyValue_InContainer(Message);
				ValueString = FString::Printf(TEXT("%d"), V);
			}
			else if (FByteProperty* ByteProperty = CastField<FByteProperty>(Property))
			{
				uint8 V = ByteProperty->GetPropertyValue_InContainer(Message);
				ValueString = FString::Printf(TEXT("%d"), V);
			}
			else if (FInt16Property* Int16Property = CastField<FInt16Property>(Property))
			{
				int16 V = Int16Property->GetPropertyValue_InContainer(Message);
				ValueString = FString::Printf(TEXT("%d"), V);
			}
			else if (FUInt16Property* Uint16Property = CastField<FUInt16Property>(Property))
			{
				uint16 V = Uint16Property->GetPropertyValue_InContainer(Message);
				ValueString = FString::Printf(TEXT("%d"), V);
			}
			else if (FIntProperty* IntProperty = CastField<FIntProperty>(Property))
			{
				int32 V = IntProperty->GetPropertyValue_InContainer(Message);
				ValueString = FString::Printf(TEXT("%d"), V);
			}
			else if (FUInt32Property* Uint32roperty = CastField<FUInt32Property>(Property))
			{
				uint32 V = Uint32roperty->GetPropertyValue_InContainer(Message);
				ValueString = FString::Printf(TEXT("%u"), V);
			}
			else if (FInt64Property* Int64Property = CastField<FInt64Property>(Property))
			{
				int64 V = Int64Property->GetPropertyValue_InContainer(Message);
				ValueString = FString::Printf(TEXT("%lld"), V);
			}
			else if (FUInt64Property* Uint64Property = CastField<FUInt64Property>(Property))
			{
				uint64 V = Uint64Property->GetPropertyValue_InContainer(Message);
				ValueString = FString::Printf(TEXT("%llu"), V);
			}
			else if (FFloatProperty* FloatProperty = CastField<FFloatProperty>(Property))
			{
				float V = FloatProperty->GetPropertyValue_InContainer(Message);
				ValueString = FString::Printf(TEXT("%.6e"), V);
			}
			else if (FDoubleProperty* DoubleProperty = CastField<FDoubleProperty>(Property))
			{
				double V = DoubleProperty->GetPropertyValue_InContainer(Message);
				//ValueString = FString::Printf(TEXT("%f"), V);
				ValueString = FString::Printf(TEXT("%.6e"), V);
			}
			else if (FStrProperty* StrProperty = CastField<FStrProperty>(Property))
			{
				FString V = StrProperty->GetPropertyValue_InContainer(Message);
				ValueString = FString::Printf(TEXT("%s"), *V);
			}
			else if (FEnumProperty* EnumProperty = CastField<FEnumProperty>(Property))
			{
				// FEnumProperty 的底層值包在一個更內層的位置，先取得ValuePtr才轉到正確位址
				const void* ValuePtr = EnumProperty->ContainerPtrToValuePtr<void>(Message);
				FNumericProperty* UnderlyingProperty = EnumProperty->GetUnderlyingProperty();
				int64 EnumValue = EnumProperty->GetUnderlyingProperty()->GetSignedIntPropertyValue(ValuePtr);
				UEnum* EnumDef = EnumProperty->GetEnum();
				FString EnumName = EnumDef->GetNameStringByValue(EnumValue);
				ValueString = FString::Printf(TEXT("%s(%llu)"), *EnumName, EnumValue);
			}

			UE_LOG(LogTemp, Log, TEXT(" (%s)%s = %s"), *TypeName, *PropertyName, *ValueString);
		}
	}
}
