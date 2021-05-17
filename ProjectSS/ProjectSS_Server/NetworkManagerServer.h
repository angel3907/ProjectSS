#pragma once
#include "NetworkManager.h"
#include "InputMemoryBitStream.h"
#include "SocketAddress.h"
#include <unordered_map>
#include "ClientProxy.h"

class NetworkManagerServer : public NetworkManager
{
public:
	static NetworkManagerServer* sInstance;
	virtual ~NetworkManagerServer(){}

	//네트워크 매니저 초기화
	static bool StaticInit(uint16_t InPort);

	//패킷 송신
	void SendOutgoingPackets();
	
	//패킷 처리
	virtual void ProcessPacket(InputMemoryBitStream& InInputStream, const SocketAddress& InFromAddress) override;

	//특정 클라이언트 프록시 포인터 가져오기
	ClientProxyPtr GetClientProxy (int PlayerId) const;

	//모든 클라이언트 업데이트
	void UpdateAllClients();

	//모든 클라이언트에 리플리케이션 패킷 추가
	void AddUnprocessedRAToAllClients(ReplicationCommand& RA);

private:
	NetworkManagerServer();

	//새 클라이언트로부터 온 패킷 처리 (헬로 패킷)
	void HandlePacketFromNewClient(InputMemoryBitStream& InInputStream, const SocketAddress& InFromAddress);

	//패킷 처리 (입력 패킷)
	void ProcessPacket(ClientProxyPtr InClientProxy, InputMemoryBitStream& InInputStream);

	//웰컴 패킷 송신
	void SendWelcomePacket(ClientProxyPtr InClientProxy);

	//상태 패킷 송신
	void SendStatePacketToClient(ClientProxyPtr InClientProxy);

	//받은 입력 패킷 처리
	void HandleInputPacket(ClientProxyPtr InClientProxy, InputMemoryBitStream& InInputStream);

	typedef std::unordered_map <int, ClientProxyPtr> IntToClientMap;
	typedef std::unordered_map <SocketAddress, ClientProxyPtr> AddressToClientMap;

	//소켓 주소 - 클라이언트 프록시 맵
	AddressToClientMap mAddressToClientMap;

	//플레이어 아이디 - 클라이언트 프록시 맵
	IntToClientMap mPlayerIdToClientMap;

	//새로 할당할 플레이어 아이디
	int mNewPlayerId;

	//상태 패킷 보내는 주기
	float mTimeBetweenStatePackets;

	//상태 패킷을 마지막으로 보낸 시간
	float mTimeOfLastStatePackets;
};