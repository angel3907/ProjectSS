#pragma once
#include "stdafx.h"

class SocketAddressFactory
{
public:
	static SocketAddressPtr CreateIPv4FromIPString(const string& InString); //인자는 IP주소+포트번호
	static SocketAddressPtr CreateIPv4FromDomainString(const string& inString); //인자는 호스트이름+포트번호
};