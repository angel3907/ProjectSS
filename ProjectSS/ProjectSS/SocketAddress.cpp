#include "stdafx.h"

SocketAddress::SocketAddress(uint32_t InAddress, uint16_t InPort)
{
	GetAsSockAddrIn()->sin_family = AF_INET;
	GetAsSockAddrIn()->sin_addr.S_un.S_addr = htonl(InAddress);
	GetAsSockAddrIn()->sin_port = htons(InPort);
}

SocketAddress::SocketAddress(const sockaddr& inSockAddr)
{
	memcpy(&mSockAddr, &inSockAddr, sizeof(sockaddr));
}

SocketAddress::SocketAddress()
{
	GetAsSockAddrIn()->sin_family = AF_INET;
	GetAsSockAddrIn()->sin_addr.S_un.S_addr = INADDR_ANY;
	GetAsSockAddrIn()->sin_port = 0;
}

size_t SocketAddress::GetSize() const
{
	return sizeof(sockaddr);
}

std::string SocketAddress::ToString() const
{
	const sockaddr_in* SockAddrIn = GetAsSockAddrIn();
	in_addr SockAddr;
	SockAddr.S_un.S_addr = ntohl(SockAddrIn->sin_addr.S_un.S_addr); //호스트기준 IP주소로 보여주기

	int b1 = SockAddr.S_un.S_un_b.s_b1;
	int b2 = SockAddr.S_un.S_un_b.s_b2;
	int b3 = SockAddr.S_un.S_un_b.s_b3;
	int b4 = SockAddr.S_un.S_un_b.s_b4;

	int port = ntohs(SockAddrIn->sin_port); //호스트기준 포트번호로 보여주기

	std::string IpAddress = std::to_string(b1) + ',' + std::to_string(b2) + ',' + std::to_string(b3) + ',' + std::to_string(b4) + ':' + std::to_string(port);
	return IpAddress;
}

bool SocketAddress::operator==(const SocketAddress& InOther) const
{
	return (mSockAddr.sa_family == AF_INET &&
		GetAsSockAddrIn()->sin_port == InOther.GetAsSockAddrIn()->sin_port) &&
		(GetIP4Ref() == InOther.GetIP4Ref());
}

uint32_t& SocketAddress::GetIP4Ref()
{
	return *reinterpret_cast<uint32_t*> (&GetAsSockAddrIn()->sin_addr.S_un.S_addr);
}

const uint32_t& SocketAddress::GetIP4Ref() const
{
	return *reinterpret_cast<const uint32_t*> (&GetAsSockAddrIn()->sin_addr.S_un.S_addr);
}

size_t SocketAddress::GetHash() const
{
	//IP주소, 포트, 주소패밀리 값을 or 연산
	return (GetIP4Ref()) |
		((static_cast<uint32_t>(GetAsSockAddrIn()->sin_port)) << 13) |
		mSockAddr.sa_family;
}

sockaddr_in* SocketAddress::GetAsSockAddrIn()
{
	return reinterpret_cast<sockaddr_in*>(&mSockAddr);
}

const sockaddr_in* SocketAddress::GetAsSockAddrIn() const
{
	return reinterpret_cast<const sockaddr_in*>(&mSockAddr);
}