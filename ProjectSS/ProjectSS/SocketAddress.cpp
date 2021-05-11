#include "stdafx.h"

string SocketAddress::ToString() const
{
	const sockaddr_in* SockAddrIn = GetAsSockAddrIn();
	in_addr SockAddr;
	SockAddr.S_un.S_addr = ntohl(SockAddrIn->sin_addr.S_un.S_addr); //ȣ��Ʈ���� IP�ּҷ� �����ֱ�

	int b1 = SockAddr.S_un.S_un_b.s_b1;
	int b2 = SockAddr.S_un.S_un_b.s_b2;
	int b3 = SockAddr.S_un.S_un_b.s_b3;
	int b4 = SockAddr.S_un.S_un_b.s_b4;

	int port = ntohs(SockAddrIn->sin_port); //ȣ��Ʈ���� ��Ʈ��ȣ�� �����ֱ�

	string IpAddress = to_string(b1) + ',' + to_string(b2) + ',' + to_string(b3) + ',' + to_string(b4) + ':' + to_string(port);
	return IpAddress;
}