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

//RPC ����� �Լ� - ����ȭ�� �����͸� Ǯ�� �ش� �Լ� ���ڷ� �Ѱ���
using RPCUnwrapFunc = void (*) (InputMemoryBitStream&);

//RPC ���� �Լ� - ���� �����͸� ����ȭ��Ŵ
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
		//�̹� ��ϵǾ������� ȣ���ϸ� �ȵ�
		assert(mRPCIdToRPCUnWrapTable.find(InRPCId) == mRPCIdToRPCUnWrapTable.end());
		mRPCIdToRPCUnWrapTable[InRPCId] = InFunc;
	}

	void RegisterWrapFunction(uint32_t InRPCId, RPCWrapFunc InFunc)
	{
		//�̹� ��ϵǾ������� ȣ���ϸ� �ȵ�
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

	//RPC �ĺ��ڴ� 4���� ���ͷ�, ��ȣ ���� ���� �ĺ��ڷ� ���е�.
	std::unordered_map<uint32_t, RPCUnwrapFunc> mRPCIdToRPCUnWrapTable;
	std::unordered_map<uint32_t, RPCWrapFunc> mRPCIdToRPCWrapTable;
};