#pragma once
#include <assert.h>
#include <stdint.h>
#include <unordered_map>
#include "InputMemoryBitStream.h"
#include "LinkingContext.h"
#include "GameObject.h"

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

//RMI ����� �Լ� - ����ȭ�� �����͸� Ǯ�� �ش� �Լ� ���ڷ� �Ѱ��� //+ bool�� ������ �ش� �Լ��� ȣ������ (�о���⸸ �ϴ� �뵵)
template <class T>
using RMIUnwrapFunc = void (T::*) (InputMemoryBitStream&, bool bFuncCall);

//RMI ���� �Լ� - ���� �����͸� ����ȭ��Ŵ
template <class T>
using RMIWrapFunc = void (T::*) (OutputMemoryBitStream&, RPCParams*);

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

	void RegisterUnwrapFunction(uint32_t InRPCId, RMIUnwrapFunc<GameObject> InFunc)
	{
		//�̹� ��ϵǾ������� ȣ���ϸ� �ȵ�
		assert(mRPCIdToRMIUnWrapTable.find(InRPCId) == mRPCIdToRMIUnWrapTable.end());
		mRPCIdToRMIUnWrapTable[InRPCId] = InFunc;
	}

	void RegisterWrapFunction(uint32_t InRPCId, RMIWrapFunc<GameObject> InFunc)
	{
		//�̹� ��ϵǾ������� ȣ���ϸ� �ȵ�
		assert(mRPCIdToRMIWrapTable.find(InRPCId) == mRPCIdToRMIWrapTable.end());
		mRPCIdToRMIWrapTable[InRPCId] = InFunc;
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

	//��� �Լ� �����͸� �����;� ��
	void ProcessRMI(InputMemoryBitStream& InStream, uint32_t InNetworkId)
	{
		uint32_t RPCId;
		InStream.Read(RPCId);

		GameObject* GameObj = LinkingContext::Get().GetGameObject(InNetworkId);
		if (GameObj)
		{
			(GameObj->*mRPCIdToRMIUnWrapTable[RPCId])(InStream, true);
		}
		else
		{
			GameObj = new GameObject();
			(GameObj->*mRPCIdToRMIUnWrapTable[RPCId])(InStream, false);
			delete GameObj;
		}
	}

	void ProcessRMI(OutputMemoryBitStream& InStream, RPCParams* InRPCParams, GameObject* InGameObject)
	{
		(InGameObject->*mRPCIdToRMIWrapTable[InRPCParams->GetRPCId()])(InStream, InRPCParams);
	}

	//RPC �ĺ��ڴ� 4���� ���ͷ�, ��ȣ ���� ���� �ĺ��ڷ� ���е�.
	std::unordered_map<uint32_t, RPCUnwrapFunc> mRPCIdToRPCUnWrapTable;
	std::unordered_map<uint32_t, RPCWrapFunc> mRPCIdToRPCWrapTable;

	//�ϴ� ���ӿ�����Ʈ��. �� �ܴ� ���� �߰��ؾ� �� ��. �� �Ϲ�ȭ�Ϸ��� ��� �ұ�..
	std::unordered_map<uint32_t, RMIUnwrapFunc<GameObject>> mRPCIdToRMIUnWrapTable;
	std::unordered_map<uint32_t, RMIWrapFunc<GameObject>> mRPCIdToRMIWrapTable;
};