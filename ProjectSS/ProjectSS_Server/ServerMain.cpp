#include "stdafx.h"
#include "Server.h"

int main(int argc, char **argv)
{
	if (Server::StaticInit())
	{
		Server::sInstance->Run();
	}

	return 0;
}