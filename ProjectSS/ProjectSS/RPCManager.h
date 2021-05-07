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

//RPC 도우미 함수 - 직렬화된 데이터를 풀어 해당 함수 인자로 넘겨줌
using RPCUnwrapFunc = void (*) (InputMemoryBitStream&);

//RPC 래핑 함수 - 인자 데이터를 직렬화시킴
using RPCWrapFunc = void (*) (OutputMemoryBitStream&, RPCParams*);

//RMI 도우미 함수 - 직렬화된 데이터를 풀어 해당 함수 인자로 넘겨줌 //+ bool은 실제로 해당 함수를 호출할지 (읽어오기만 하는 용도)
template <class T>
using RMIUnwrapFunc = void (T::*) (InputMemoryBitStream&, bool bFuncCall);

//RMI 래핑 함수 - 인자 데이터를 직렬화시킴
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

	void RegisterUnwrapFunction(uint32_t InRPCId, RMIUnwrapFunc<GameObject> InFunc)
	{
		//이미 등록되어있으면 호출하면 안됨
		assert(mRPCIdToRMIUnWrapTable.find(InRPCId) == mRPCIdToRMIUnWrapTable.end());
		mRPCIdToRMIUnWrapTable[InRPCId] = InFunc;
	}

	void RegisterWrapFunction(uint32_t InRPCId, RMIWrapFunc<GameObject> InFunc)
	{
		//이미 등록되어있으면 호출하면 안됨
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

	//멤버 함수 포인터를 가져와야 함
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

	//RPC 식별자는 4글자 리터럴, 부호 없는 정수 식별자로 구분됨.
	std::unordered_map<uint32_t, RPCUnwrapFunc> mRPCIdToRPCUnWrapTable;
	std::unordered_map<uint32_t, RPCWrapFunc> mRPCIdToRPCWrapTable;

	//일단 게임오브젝트만. 그 외는 새로 추가해야 할 듯. 더 일반화하려면 어떻게 할까..
	std::unordered_map<uint32_t, RMIUnwrapFunc<GameObject>> mRPCIdToRMIUnWrapTable;
	std::unordered_map<uint32_t, RMIWrapFunc<GameObject>> mRPCIdToRMIWrapTable;
};