// Copyright Epic Games, Inc. All Rights Reserved.

// Self
#include "ProtoSampleGameMode.h"
// UE
#include "UObject/ConstructorHelpers.h"
// ProtoSample
#include "Message/TestMessage1.pb.h"
#include "Message/TestMessage2.pb.h"
#include "MessageObject/TestMessage3.h"
#include "MessageObject/TestMessage4.h"
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
	if (TestMessage3_Class)
	{
		TestMessage3 = UTestMessage3::MakeMessageObject(this, TestMessage3_Class.Get());
		TestMessage3->NativeMessage = UTestMessage3::MakeMessage();

		TestMessage3->InitMessage();
		TestMessage3->InitNativeMessage();

		UE_LOG(LogTemp, Log, TEXT("Create %s"), *TestMessage3->GetName());
		DumpMessageProperty(TestMessage3);

		const google::protobuf::Descriptor* Descriptor = TestMessage3->NativeMessage->GetDescriptor();
		const google::protobuf::Reflection* Reflection = TestMessage3->NativeMessage->GetReflection();
		UE_LOG(LogTemp, Log, TEXT(" %s's field ==="), UTF8_TO_TCHAR(Descriptor->name().c_str()));
		DumpMessageField(*TestMessage3->NativeMessage.Get());
	}
}

void AProtoSampleGameMode::CreateTestMessage4()
{
	if (TestMessage4_Class)
	{
		TestMessage4 = NewObject<UTestMessage4>(this, TestMessage4_Class.Get());
		TestMessage4->InitMessage();

		UE_LOG(LogTemp, Log, TEXT("Create %s"), *TestMessage4->GetName());
		DumpObjectProperty(TestMessage4);

		ProtoTestMessage4 = MakeUnique<protobuf::TestMessage4>();
		const google::protobuf::Descriptor* Descriptor = ProtoTestMessage4->GetDescriptor();
		UE_LOG(LogTemp, Log, TEXT("Create %s"), UTF8_TO_TCHAR(Descriptor->name().c_str()));
		DumpObjectField(*ProtoTestMessage4.Get());
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

void AProtoSampleGameMode::DumpObjectProperty(UObject* Message)
{
	if (!IsValid(Message))
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT(" %s's object property ==="), *Message->GetName());

	for (TFieldIterator<FProperty> PropertyIt(Message->GetClass()); PropertyIt; ++PropertyIt)
	{
		FProperty* Property = *PropertyIt;

		if (FIntProperty* IntProperty = CastField<FIntProperty>(Property))
		{
			FString TypeName = Property->GetCPPType();
			FString PropertyName = Property->GetName();
			FString ValueString = FString::Printf(TEXT("%d"), IntProperty->GetPropertyValue_InContainer(Message));
			
			UE_LOG(LogTemp, Log, TEXT(" (%s)%s = %s"), *TypeName, *PropertyName, *ValueString);
		}
		else if (FObjectProperty* ObjectProperty = CastField<FObjectProperty>(Property))
		{
			void* ValuePtr = ObjectProperty->ContainerPtrToValuePtr<void>(Message);
			UObject* ReferencedObject = ObjectProperty->GetObjectPropertyValue(ValuePtr);

			if (ReferencedObject)
			{
				FString TypeName = ReferencedObject->GetClass()->GetName();
				FString PropertyName = Property->GetName();

				UE_LOG(LogTemp, Log, TEXT(" (%s)%s = "), *TypeName, *PropertyName);

				DumpObjectProperty(ReferencedObject);
			}
		}
	}
}

void AProtoSampleGameMode::DumpMessageField(google::protobuf::Message& Message)
{
	const google::protobuf::Descriptor* Descriptor = TestMessage3->NativeMessage->GetDescriptor();
	const google::protobuf::Reflection* Reflection = TestMessage3->NativeMessage->GetReflection();

	int32 FieldCount = Descriptor->field_count();
	for (int32 i = 0; i < FieldCount; i++)
	{
		const google::protobuf::FieldDescriptor* Field = Descriptor->field(i);

		FString TypeName = UTF8_TO_TCHAR(Field->cpp_type_name());
		FString FieldName = UTF8_TO_TCHAR(Field->name().c_str());
		FString ValueString = "";

		switch (Field->cpp_type())
		{
		case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
		{
			bool V = Reflection->GetBool(Message, Field);
			ValueString = FString::Printf(TEXT("%s"), V ? TEXT("true") : TEXT("false"));
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
		{
			int32 V = Reflection->GetInt32(Message, Field);
			ValueString = FString::Printf(TEXT("%d"), V);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
		{
			uint32 V = Reflection->GetUInt32(Message, Field);
			ValueString = FString::Printf(TEXT("%u"), V);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
		{
			int64 V = Reflection->GetInt64(Message, Field);
			ValueString = FString::Printf(TEXT("%lld"), V);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
		{
			uint64 V = Reflection->GetUInt64(Message, Field);
			ValueString = FString::Printf(TEXT("%llu"), V);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
		{
			float V = Reflection->GetFloat(Message, Field);
			ValueString = FString::Printf(TEXT("%.6e"), V);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
		{
			double V = Reflection->GetDouble(Message, Field);
			ValueString = FString::Printf(TEXT("%.6e"), V);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
		{
			FString V = UTF8_TO_TCHAR(Reflection->GetString(Message, Field).c_str());
			ValueString = FString::Printf(TEXT("%s"), *V);
		}
		break;
		case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
		{
			const google::protobuf::EnumDescriptor* EnumDesc = Field->enum_type();
			TypeName = UTF8_TO_TCHAR(EnumDesc->name().c_str());

			const google::protobuf::EnumValueDescriptor* EnumValueDesc = Reflection->GetEnum(Message, Field); 
			FString EnumName = UTF8_TO_TCHAR(EnumValueDesc->name().c_str());
			int32 EnumValue = EnumValueDesc->number();
			ValueString = FString::Printf(TEXT("%s(%d)"), *EnumName, EnumValue);
		}
		break;
		}

		UE_LOG(LogTemp, Log, TEXT(" (%s)%s = %s"), *TypeName, *FieldName, *ValueString);
	}
}

void AProtoSampleGameMode::DumpObjectField(google::protobuf::Message& Message)
{
	const google::protobuf::Descriptor* Descriptor = TestMessage3->NativeMessage->GetDescriptor();
	const google::protobuf::Reflection* Reflection = TestMessage3->NativeMessage->GetReflection();

	int32 FieldCount = Descriptor->field_count();
	for (int32 i = 0; i < FieldCount; i++)
	{
		const google::protobuf::FieldDescriptor* Field = Descriptor->field(i);

		FString TypeName = UTF8_TO_TCHAR(Field->cpp_type_name());
		FString FieldName = UTF8_TO_TCHAR(Field->name().c_str());
		FString ValueString = "";

		if (Field->cpp_type() == google::protobuf::FieldDescriptor::CPPTYPE_INT32)
		{
			int32 V = Reflection->GetInt32(Message, Field);
			ValueString = FString::Printf(TEXT("%d"), V);
			UE_LOG(LogTemp, Log, TEXT(" (%s)%s = %s"), *TypeName, *FieldName, *ValueString);
		}
		else if (Field->cpp_type() == google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE)
		{
			if (!Field->is_repeated() && Reflection->HasField(Message, Field))
			{
				UE_LOG(LogTemp, Log, TEXT(" (%s)%s = "), *TypeName, *FieldName);

				google::protobuf::Message* SubMsg = Reflection->MutableMessage(&Message, Field);
				DumpObjectField(*SubMsg);
			}
		}
	}
}
