#pragma once
#include "ObjectCreationRegistry.h"
#include "MathUtils.h"
#include "RPCManager.h"
#include "ReplicationHeader.h"

namespace GamePlayUtils
{ 
	inline void StartGame()
	{
		SocketUtil::StartUsingSocket();
		ObjectCreationRegistry::RegisterObjectCreation();
	}

	inline void EndGame()
	{
		SocketUtil::EndUsingSocket();
	}
}


static void PrintSomething(const string& InName, const Vector2 InLocation, float InVolume)
{
	printf("Hi, My name is %s, My Location is (%f, %f), My Volume is %f", InName.c_str(), InLocation.PosX, InLocation.PosY, InVolume);
}

struct PrintSomethingRPCParams
{
	string mName;
	Vector2 mLocation;
	float mVolume;
};

static void UnwrapPrintSomething(InputMemoryBitStream& InStream)
{
	string Name = "";
	Vector2 Location = {0, 0};
	float Volume = 0;

	InStream.Read(Name);
	InStream.ReadPosF(Location);
	InStream.Read(Volume);

	PrintSomething(Name, Location, Volume);
}

static void RegisterRPCs()
{
	RPCManager::Get().RegisterUnwrapFunction('PRST', UnwrapPrintSomething);
}

void PrintSomething(OutputMemoryBitStream& InStream, const string& InName, const Vector2 InLocation, float InVolume)
{
	ReplicationHeader Rh(ReplicationAction::RA_RPC);
	Rh.Write(InStream);

	InStream.Write(InName);
	InStream.WritePosF(InLocation);
	InStream.Write(InVolume);
}