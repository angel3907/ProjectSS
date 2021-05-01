#include "stdafx.h"

UDPSocketPtr SocketUtil::CreateUDPSocket(SocketAddressFamily inFamily)
{
	//������ ��ȿ�� ���� �����ϵ��� ��.
	//->UDPSocket�� �����Ѵٸ�, mSocket�� �ݵ�� �������.
	SOCKET s = socket(static_cast<int>(inFamily), SOCK_DGRAM, IPPROTO_UDP);
	if (s != INVALID_SOCKET)
	{
		return UDPSocketPtr(new UDPSocket(s));
	}

	ReportError("SocketUtil::CreateUDPSocket");
	return nullptr;
}


TCPSocketPtr SocketUtil::CreateTCPSocket(SocketAddressFamily inFamily)
{
	//������ ��ȿ�� ���� �����ϵ��� ��.
	//->TCPSocket�� �����Ѵٸ�, mSocket�� �ݵ�� �������.
	SOCKET s = socket(static_cast<int>(inFamily), SOCK_STREAM, IPPROTO_TCP);
	if (s != INVALID_SOCKET)
	{
		return TCPSocketPtr(new TCPSocket(s));
	}

	ReportError("SocketUtil::CreateTCPSocket");
	return nullptr;
}

void SocketUtil::ReportError(const char* inOperationDesc)
{
	LPVOID lpMsgBuf;
	DWORD errorNum = GetLastError();

	//DWORD�� �Ѿ�� ������ȣ�� ���ڿ��� �ٲ���.
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	LOG("Error %s: %d- %s", inOperationDesc, errorNum, lpMsgBuf);
}

int SocketUtil::GetLastError()
{
	return WSAGetLastError();
}
