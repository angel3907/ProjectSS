#pragma once
#include <assert.h>
#include <stdint.h>
#include <unordered_map>
#include "InputMemoryBitStream.h"
class RPCManager;

class RPCParams
{
private:
	uint32_t RPCId = 0;

public:
	RPCParams(uint32_t RPCId) : RPCId(RPCId) {}
	virtual ~RPCParams() {};
	uint32_t GetRPCId() { return RPCId; }
};

//RPC 도우미 함수 - 직렬화된 데이터를 풀어 해당 함수 인자로 넘겨줌
using RPCUnwrapFunc = void (*) (InputMemoryBitStream&);

//RPC 래핑 함수 - 인자 데이터를 직렬화시킴
using RPCWrapFunc = void (*) (OutputMemoryBitStream&, RPCParams*);

class RPCManager
{
public:
	static RPCManager& Get()
	{
		static RPCManager sInstance;
		return sInstance;
	}

	void RegisterUnwrapFunction(uint32_t InRPCId, RPCUnwrapFunc InFunc)
	{
		//이미 등록되어있으면 호출하면 안됨
		assert(mRPCIdToRPCUnWrapTable.find(InRPCId) == mRPCIdToRPCUnWrapTable.end());
		mRPCIdToRPCUnWrapTable[InRPCId] = InFunc;
	}

	void RegisterWrapFunction(uint32_t InRPCId, RPCWrapFunc InFunc)
	{
		//이미 등록되어있으면 호출하면 안됨
		assert(mRPCIdToRPCWrapTable.find(InRPCId) == mRPCIdToRPCWrapTable.end());
		mRPCIdToRPCWrapTable[InRPCId] = InFunc;
	}

	void ProcessRPC(InputMemoryBitStream& InStream)
	{
		uint32_t RPCId;
		InStream.Read(RPCId);
		mRPCIdToRPCUnWrapTable[RPCId](InStream);
	}

	void ProcessRPC(OutputMemoryBitStream& InStream, RPCParams* InRPCParams)
	{
		mRPCIdToRPCWrapTable[InRPCParams->GetRPCId()](InStream, InRPCParams);
	}

	//RPC 식별자는 4글자 리터럴, 부호 없는 정수 식별자로 구분됨.
	std::unordered_map<uint32_t, RPCUnwrapFunc> mRPCIdToRPCUnWrapTable;
	std::unordered_map<uint32_t, RPCWrapFunc> mRPCIdToRPCWrapTable;
};