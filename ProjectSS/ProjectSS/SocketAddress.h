#pragma once
#include <stdint.h> //for using uint32_t, uint16_t
#include <memory> //for using shared_ptr
using namespace std; //for using ""

//자료형 안전성이 확보된 상태로 소켓 기본 자료형 구현
class SocketAddress
{
public:
	SocketAddress(uint32_t InAddress, uint16_t InPort) //IP주소, Port로 객체 생성
	{
		GetAsSockAddrIn()->sin_family = AF_INET;
		GetAsSockAddrIn()->sin_addr.S_un.S_addr = htonl(InAddress);
		GetAsSockAddrIn()->sin_port = htons(InPort);
	}

	SocketAddress(const sockaddr& inSockAddr) //sockaddr 구조체로 객체 생성
	{
		memcpy(&mSockAddr, &inSockAddr, sizeof(sockaddr));
	}

	size_t GetSize() const { return sizeof(sockaddr);} //객체의 크기

private:
	friend class UDPSocket;

	sockaddr mSockAddr;
	sockaddr_in* GetAsSockAddrIn() // IPV4 전용 소켓주소 구조체 반환
	{
		return reinterpret_cast<sockaddr_in*>(&mSockAddr);
	}
};

using SocketAddressPtr = shared_ptr<SocketAddress>; //선언해두면 소켓 주소를 여러곳에서 공유해서 쓸 때 메모리 정리를 신경안써도 됨.