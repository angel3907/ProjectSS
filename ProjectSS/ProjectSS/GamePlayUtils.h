#pragma once
#include "ObjectCreationRegistry.h"
#include "MathUtils.h"
#include "RPCManager.h"
#include "ReplicationHeader.h"

struct PrintSomethingRPCParams : public RPCParams
{
	string mName;
	Vector2 mLocation;
	float mVolume;

	PrintSomethingRPCParams(const string& InName, Vector2 InLocation, float InVolume) 
	//: RPCParams('PRST'), mName(InName), mLocation(InLocation), mVolume(InVolume) {}
	: RPCParams('GPRT'), mName(InName), mLocation(InLocation), mVolume(InVolume) {}
};

static void PrintSomething(const string& InName, const Vector2 InLocation, float InVolume)
{
	printf("Hi, My name is %s, My Location is (%f, %f), My Volume is %f", InName.c_str(), InLocation.PosX, InLocation.PosY, InVolume);
}

static void UnwrapPrintSomething(InputMemoryBitStream& InStream)
{
	string Name = "";
	Vector2 Location = { 0, 0 };
	float Volume = 0;

	InStream.Read(Name);
	InStream.ReadPosF(Location);
	InStream.Read(Volume);

	PrintSomething(Name, Location, Volume);
}

static void PrintSomething(OutputMemoryBitStream& InStream, RPCParams* InRPCParams)
{
	PrintSomethingRPCParams* RealTypeParam = dynamic_cast<PrintSomethingRPCParams*>(InRPCParams);

	if (RealTypeParam == nullptr)
	{
		printf("RPCParam has different Type Value");
		return;
	}

	InStream.Write(RealTypeParam->mName);
	InStream.WritePosF(RealTypeParam->mLocation);
	InStream.Write(RealTypeParam->mVolume);
}

namespace GamePlayUtils
{ 
	//RPC 사용 전 반드시 등록
	inline void RegisterRPCs()
 	{
// 		RPCManager::Get().RegisterUnwrapFunction('PRST', UnwrapPrintSomething);
// 		RPCManager::Get().RegisterWrapFunction('PRST', PrintSomething);
// 		
		RPCManager::Get().RegisterUnwrapFunction('GPRT', &GameObject::UnwrapPrintSomething);
		RPCManager::Get().RegisterWrapFunction('GPRT', &GameObject::PrintSomething);
	}

	inline void StartGame()
	{
		SocketUtil::StartUsingSocket();
		ObjectCreationRegistry::RegisterObjectCreation();
		RegisterRPCs();
	}

	inline void EndGame()
	{
		SocketUtil::EndUsingSocket();
	}
}