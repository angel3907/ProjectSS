#include "stdafx.h"
#include "Server.h"

int main()
{
	if (Server::StaticInit())
	{
		Server::sInstance->Run();
	}
}