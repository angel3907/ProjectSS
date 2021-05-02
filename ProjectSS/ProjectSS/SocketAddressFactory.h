#pragma once
#include "stdafx.h"

class SocketAddressFactory
{
public:
	static SocketAddressPtr CreateIPv4FromIPString(const string& InString); //���ڴ� IP�ּ�+��Ʈ��ȣ
	static SocketAddressPtr CreateIPv4FromDomainString(const string& inString); //���ڴ� ȣ��Ʈ�̸�+��Ʈ��ȣ
};