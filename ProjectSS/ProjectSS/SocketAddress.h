#pragma once
#include <stdint.h> //for using uint32_t, uint16_t
#include <memory> //for using shared_ptr
#include <string>
#include <ws2def.h>

struct sockaddr;
struct sockaddr_in;

//자료형 안전성이 확보된 상태로 소켓 기본 자료형 구현
class SocketAddress
{
public:
	SocketAddress(uint32_t InAddress, uint16_t InPort);  //IP주소, Port로 객체 생성
	SocketAddress(const sockaddr& inSockAddr); //sockaddr 구조체로 객체 생성
	SocketAddress(); //기본 생성자

	size_t GetSize() const; //객체의 크기
	std::string ToString() const;

	bool operator==(const SocketAddress& InOther) const;

	//uin32_t 형태로 IPV4 주소를 리턴해주는 함수
	uint32_t& GetIP4Ref();
	const uint32_t& GetIP4Ref() const;

	//해쉬값 얻어오는 함수 (for unordered_map)
	size_t GetHash() const;

private:
	friend class UDPSocket;
	friend class TCPSocket;

	sockaddr mSockAddr;
	sockaddr_in* GetAsSockAddrIn(); // IPV4 전용 소켓주소 구조체 반환
	const sockaddr_in* GetAsSockAddrIn() const; // IPV4 전용 소켓주소 구조체 반환
	
};

using SocketAddressPtr = std::shared_ptr<SocketAddress>; //소켓 주소를 여러곳에서 공유해서 쓸 때 메모리 정리 신경안써도 됨.

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