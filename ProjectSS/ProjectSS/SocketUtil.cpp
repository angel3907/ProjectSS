#include "stdafx.h"

UDPSocketPtr SocketUtil::CreateUDPSocket(SocketAddressFamily inFamily)
{
	//소켓이 유효할 때만 생성하도록 함.
	//->UDPSocket이 존재한다면, mSocket은 반드시 살아있음.
	SOCKET s = socket(static_cast<int>(inFamily), SOCK_DGRAM, IPPROTO_UDP);
	if (s != INVALID_SOCKET)
	{
		return UDPSocketPtr(new UDPSocket(s));
	}

	ReportError("SocketUtil::CreateUDPSocket");
	return nullptr;
}

void SocketUtil::ReportError(const char* inOperationDesc)
{
	LPVOID lpMsgBuf;
	DWORD errorNum = GetLastError();

	//DWORD로 넘어온 에러번호를 문자열로 바꿔줌.
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);


	printf("Error %s: %d- %s", inOperationDesc, errorNum, static_cast<const char*>(lpMsgBuf));
}

int SocketUtil::GetLastError()
{
	return WSAGetLastError();
}
