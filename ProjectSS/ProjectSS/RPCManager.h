#pragma once
#include <assert.h>
#include <stdint.h>
#include <unordered_map>
#include "InputMemoryBitStream.h"

//RPC ����� �Լ� - ����ȭ�� �����͸� Ǯ�� �ش� �Լ� ���ڷ� �Ѱ���
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
		//�̹� ��ϵǾ������� ȣ���ϸ� �ȵ�
		assert(mRPCIdToRPCTable.find(InRPCId) == mRPCIdToRPCTable.end());
		mRPCIdToRPCTable[InRPCId] = InFunc;
	}

	void ProcessRPC(InputMemoryBitStream& InStream)
	{
		uint32_t RPCId;
		InStream.Read(RPCId);
		mRPCIdToRPCTable[RPCId](InStream);
	}

	//RPC �ĺ��ڴ� 4���� ���ͷ�, ��ȣ ���� ���� �ĺ��ڷ� ���е�.
	std::unordered_map<uint32_t, RPCUnwrapFunc> mRPCIdToRPCTable;
};