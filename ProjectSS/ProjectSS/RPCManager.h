#pragma once
#include <assert.h>
#include <stdint.h>
#include <unordered_map>
#include "InputMemoryBitStream.h"

//RPC 도우미 함수 - 직렬화된 데이터를 풀어 해당 함수 인자로 넘겨줌
using RPCUnwrapFunc = void (*) (InputMemoryBitStream&);

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
		assert(mRPCIdToRPCTable.find(InRPCId) == mRPCIdToRPCTable.end());
		mRPCIdToRPCTable[InRPCId] = InFunc;
	}

	void ProcessRPC(InputMemoryBitStream& InStream)
	{
		uint32_t RPCId;
		InStream.Read(RPCId);
		mRPCIdToRPCTable[RPCId](InStream);
	}

	//RPC 식별자는 4글자 리터럴, 부호 없는 정수 식별자로 구분됨.
	std::unordered_map<uint32_t, RPCUnwrapFunc> mRPCIdToRPCTable;
};