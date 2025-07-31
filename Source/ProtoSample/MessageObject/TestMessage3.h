// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// UE
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
// ProtoSample
#include "Message/TestMessage3.pb.h"
// 
#include "TestMessage3.generated.h"

UENUM(BlueprintType)
enum class ETestType : uint8
{
	A,
	B,
	C
};

UCLASS(Blueprintable)
class PROTOSAMPLE_API UTestMessage3 : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	bool ValBool = false;

	UPROPERTY()
	int8 ValInt8 = INT8_MIN;

	UPROPERTY(BlueprintReadWrite)
	uint8 ValUint8 = UINT8_MAX;

	UPROPERTY()
	int16 ValInt16 = INT16_MIN;

	UPROPERTY()
	uint16 ValUint16 = UINT16_MAX;

	UPROPERTY(BlueprintReadWrite)
	int32 ValInt32 = INT_MIN;

	UPROPERTY()
	uint32 ValUint32 = UINT32_MAX;

	UPROPERTY(BlueprintReadWrite)
	int64 ValInt64 = INT64_MIN;

	UPROPERTY()
	uint64 ValUint64 = UINT64_MAX;

	UPROPERTY(BlueprintReadWrite)
	float ValFloat = FLT_MIN;

	UPROPERTY()
	double ValDouble = DBL_MAX;

	UPROPERTY(BlueprintReadWrite)
	FString ValString = TEXT("Cindy");

	UPROPERTY(BlueprintReadWrite)
	ETestType ValEnum = ETestType::C;

public:
	TUniquePtr<protobuf::TestMessage3> NativeMessage;
};
