#include "stdafx.h"

void GameObject::Write(OutputMemoryBitStream& InStream) const
{
	InStream.WritePosF(Pos);
}

void GameObject::Read(InputMemoryBitStream& InStream)
{
	InStream.ReadPosF(Pos);
}

void GameObject::WriteChanged(OutputMemoryBitStream& InStream) const
{
	//비트 필드부터 읽어야 실제 읽어야 할 속성을 판단 가능
	InStream.Write(mProperties, GetRequiredBits(static_cast<uint32_t>(GOP_MAX)));

	//지정된 속성만 기록함
	if ((mProperties & GOP_Pos) != 0)
		InStream.WritePosF(Pos);
}

void GameObject::ReadChanged(InputMemoryBitStream& InStream)
{
	uint32_t WrittenProperties = 0;
	InStream.Read(WrittenProperties, GetRequiredBits(static_cast<uint32_t>(GOP_MAX)));

	if ((WrittenProperties & GOP_Pos) != 0)
		InStream.ReadPosF(Pos);
}

void GameObject::PrintSomething(const string& InName, const Vector2 InLocation, float InVolume)
{
	printf("Hi, My name is %s, My Location is (%f, %f), My Volume is %f", InName.c_str(), InLocation.PosX, InLocation.PosY, InVolume);
}

void GameObject::PrintSomething(OutputMemoryBitStream& InStream, RPCParams* InRPCParams)
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

void GameObject::UnwrapPrintSomething(InputMemoryBitStream& InStream, bool bFuncCall)
{
	string Name = "";
	Vector2 Location = { 0, 0 };
	float Volume = 0;

	InStream.Read(Name);
	InStream.ReadPosF(Location);
	InStream.Read(Volume);

	PrintSomething(Name, Location, Volume);
}
