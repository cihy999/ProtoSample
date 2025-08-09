// Fill out your copyright notice in the Description page of Project Settings.

// Self
#include "TestMessage3.h"

UTestMessage3* UTestMessage3::MakeMessageObject(UObject* Outer, TSubclassOf<UTestMessage3> BlueprintClass)
{
    return NewObject<UTestMessage3>(Outer, BlueprintClass.Get());
}

TUniquePtr<protobuf::TestMessage3> UTestMessage3::MakeMessage()
{
    return MakeUnique<protobuf::TestMessage3>();
}

void UTestMessage3::InitMessage()
{
	ValBool = true;
	ValInt8 = INT8_MIN;
	ValUint8 = UINT8_MAX;
	ValInt16 = INT16_MIN;
	ValUint16 = UINT16_MAX;
	ValInt32 = INT_MIN;
	ValUint32 = UINT32_MAX;
	ValInt64 = INT64_MIN;
	ValUint64 = UINT64_MAX;
	ValFloat = FLT_MIN;
	ValDouble = DBL_MAX;
	ValString = TEXT("Cindy");
	ValEnum = ETestType::C;
}

void UTestMessage3::InitNativeMessage()
{
	NativeMessage->set_val_bool(true);
	NativeMessage->set_val_int32(INT_MIN);
	NativeMessage->set_val_uint32(UINT32_MAX);
	NativeMessage->set_val_int64(INT64_MIN);
	NativeMessage->set_val_uint64(UINT64_MAX);
	NativeMessage->set_val_float(FLT_MIN);
	NativeMessage->set_val_double(DBL_MAX);
	NativeMessage->set_val_string("Cindy");
	NativeMessage->set_val_enum(protobuf::TestType::B);
}
