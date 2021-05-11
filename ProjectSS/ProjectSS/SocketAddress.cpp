#include "stdafx.h"

string SocketAddress::ToString() const
{
	const sockaddr_in* SockAddrIn = GetAsSockAddrIn();
	in_addr SockAddr;
	SockAddr.S_un.S_addr = ntohl(SockAddrIn->sin_addr.S_un.S_addr); //호스트기준 IP주소로 보여주기

	int b1 = SockAddr.S_un.S_un_b.s_b1;
	int b2 = SockAddr.S_un.S_un_b.s_b2;
	int b3 = SockAddr.S_un.S_un_b.s_b3;
	int b4 = SockAddr.S_un.S_un_b.s_b4;

	int port = ntohs(SockAddrIn->sin_port); //호스트기준 포트번호로 보여주기

	string IpAddress = to_string(b1) + ',' + to_string(b2) + ',' + to_string(b3) + ',' + to_string(b4) + ':' + to_string(port);
	return IpAddress;
}