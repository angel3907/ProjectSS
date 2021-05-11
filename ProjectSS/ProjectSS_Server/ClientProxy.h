#pragma once
#include "memory"

class ClientProxy
{
public:
	ClientProxy(){}
	~ClientProxy(){}
};

using ClientProxyPtr = std::shared_ptr<ClientProxy>;