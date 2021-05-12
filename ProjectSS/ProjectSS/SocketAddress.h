#pragma once
#include <stdint.h> //for using uint32_t, uint16_t
#include <memory> //for using shared_ptr
#include <string>
using namespace std; //for using ""

//자료형 안전성이 확보된 상태로 소켓 기본 자료형 구현
class SocketAddress
{
public:
	SocketAddress(uint32_t InAddress, uint16_t InPort)  //IP주소, Port로 객체 생성
	{
		GetAsSockAddrIn()->sin_family = AF_INET;
		GetAsSockAddrIn()->sin_addr.S_un.S_addr = htonl(InAddress);
		GetAsSockAddrIn()->sin_port = htons(InPort);
	}

	SocketAddress(const sockaddr& inSockAddr) //sockaddr 구조체로 객체 생성
	{
		memcpy(&mSockAddr, &inSockAddr, sizeof(sockaddr));
	}

	SocketAddress() //기본 생성자
	{
		GetAsSockAddrIn()->sin_family = AF_INET;
		GetAsSockAddrIn()->sin_addr.S_un.S_addr = INADDR_ANY;
		GetAsSockAddrIn()->sin_port = 0;
	}

	size_t GetSize() const { return sizeof(sockaddr);} //객체의 크기

	string ToString() const;


	bool operator==(const SocketAddress& InOther) const
	{
		return (mSockAddr.sa_family == AF_INET && 
				GetAsSockAddrIn()->sin_port == InOther.GetAsSockAddrIn()->sin_port) &&
				(GetIP4Ref() == InOther.GetIP4Ref());
	}

	//uin32_t 형태로 IPV4 주소를 리턴해주는 함수
	uint32_t& GetIP4Ref()	{ return *reinterpret_cast<uint32_t*> (&GetAsSockAddrIn()->sin_addr.S_un.S_addr); }
	const uint32_t& GetIP4Ref() const { return *reinterpret_cast<const uint32_t*> (&GetAsSockAddrIn()->sin_addr.S_un.S_addr); }

	//해쉬값 얻어오는 함수 (for unordered_map)
	size_t GetHash() const
	{
		//IP주소, 포트, 주소패밀리 값을 or 연산
		return (GetIP4Ref()) | 
			((static_cast<uint32_t>(GetAsSockAddrIn()->sin_port)) << 13 ) |
			mSockAddr.sa_family;
	}


private:
	friend class UDPSocket;
	friend class TCPSocket;

	sockaddr mSockAddr;
	sockaddr_in* GetAsSockAddrIn() // IPV4 전용 소켓주소 구조체 반환
	{
		return reinterpret_cast<sockaddr_in*>(&mSockAddr);
	}
	const sockaddr_in* GetAsSockAddrIn() const // IPV4 전용 소켓주소 구조체 반환
	{
		return reinterpret_cast<const sockaddr_in*>(&mSockAddr);
	}
};

using SocketAddressPtr = shared_ptr<SocketAddress>; //소켓 주소를 여러곳에서 공유해서 쓸 때 메모리 정리 신경안써도 됨.

//해쉬 함수 정의
namespace std
{
	template<> 
	struct hash <SocketAddress>
	{
		size_t operator()(const SocketAddress& InSocketAddress) const
		{
			return InSocketAddress.GetHash();
		}
	
	};
}