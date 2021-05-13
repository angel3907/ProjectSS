#include "stdafx.h"
#include "Client.h"
#include "SDLRenderer.h"

int main(int argc, char **argv)
{
	if (Client::StaticInit())
	{
		Client::sInstance->Run();
	}
	
	return 0;
}