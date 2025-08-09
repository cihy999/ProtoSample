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
	int8 ValInt8 = 0;

	UPROPERTY(BlueprintReadWrite)
	uint8 ValUint8 = 0;

	UPROPERTY()
	int16 ValInt16 = 0;

	UPROPERTY()
	uint16 ValUint16 = 0;

	UPROPERTY(BlueprintReadWrite)
	int32 ValInt32 = 0;

	UPROPERTY()
	uint32 ValUint32 = 0;

	UPROPERTY(BlueprintReadWrite)
	int64 ValInt64 = 0;

	UPROPERTY()
	uint64 ValUint64 = 0;

	UPROPERTY(BlueprintReadWrite)
	float ValFloat = 0;

	UPROPERTY()
	double ValDouble = 0;

	UPROPERTY(BlueprintReadWrite)
	FString ValString = "";

	UPROPERTY(BlueprintReadWrite)
	ETestType ValEnum;

public:
	TUniquePtr<protobuf::TestMessage3> NativeMessage;

public:
	static UTestMessage3* MakeMessageObject(UObject* Outer, TSubclassOf<UTestMessage3> BlueprintClass);
	static TUniquePtr<protobuf::TestMessage3> MakeMessage();

	void InitMessage();
	void InitNativeMessage();
};
