#pragma once
#include <stdint.h> //for using uint32_t, uint16_t
#include <memory> //for using shared_ptr
using namespace std; //for using ""

//�ڷ��� �������� Ȯ���� ���·� ���� �⺻ �ڷ��� ����
class SocketAddress
{
public:
	SocketAddress(uint32_t InAddress, uint16_t InPort) //IP�ּ�, Port�� ��ü ����
	{
		GetAsSockAddrIn()->sin_family = AF_INET;
		GetAsSockAddrIn()->sin_addr.S_un.S_addr = htonl(InAddress);
		GetAsSockAddrIn()->sin_port = htons(InPort);
	}

	SocketAddress(const sockaddr& inSockAddr) //sockaddr ����ü�� ��ü ����
	{
		memcpy(&mSockAddr, &inSockAddr, sizeof(sockaddr));
	}

	SocketAddress() //�⺻ ������
	{
		GetAsSockAddrIn()->sin_family = AF_INET;
		GetAsSockAddrIn()->sin_addr.S_un.S_addr = INADDR_ANY;
		GetAsSockAddrIn()->sin_port = 0;
	}

	size_t GetSize() const { return sizeof(sockaddr);} //��ü�� ũ��

private:
	friend class UDPSocket;
	friend class TCPSocket;

	sockaddr mSockAddr;
	sockaddr_in* GetAsSockAddrIn() // IPV4 ���� �����ּ� ����ü ��ȯ
	{
		return reinterpret_cast<sockaddr_in*>(&mSockAddr);
	}
};

using SocketAddressPtr = shared_ptr<SocketAddress>; //�����صθ� ���� �ּҸ� ���������� �����ؼ� �� �� �޸� ������ �Ű�Ƚᵵ ��.