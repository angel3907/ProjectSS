#pragma once
#include "ObjectCreationRegistry.h"

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