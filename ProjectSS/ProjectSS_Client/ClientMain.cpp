#include "stdafx.h"
#include "Client.h"

int main()
{
	if (Client::StaticInit())
	{
		Client::sInstance->Run();
	}
}