#pragma once


/*
static void PrintSomething(const std::string& InName, const Vector2 InLocation, float InVolume)
{
	printf("Hi, My name is %s, My Location is (%f, %f), My Volume is %f", InName.c_str(), InLocation.PosX, InLocation.PosY, InVolume);
}

static void UnwrapPrintSomething(InputMemoryBitStream& InStream)
{
	std::string Name = "";
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
*/