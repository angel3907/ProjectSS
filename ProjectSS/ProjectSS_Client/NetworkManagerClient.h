#pragma once
#include <string>
#include "NetworkManager.h"
#include "SocketAddress.h"

class InputMemoryBitStream;

class NetworkManagerClient : public NetworkManager
{
	//클라이언트 현재 상태
	enum NetworkClientState
	{
		NCS_Uninitialized, //초기화되지 않음
		NCS_SayingHello, //헬로 패킷을 보내는 중
		NCS_Welcomed //웰컴 패킷을 받은 상태
	};

public:
	NetworkManagerClient(){}
	virtual ~NetworkManagerClient(){}
	
	//패킷을 보내고 받는 코드
	void SendOutgoingPackets();
	void ProcessPacket(InputMemoryBitStream& InInputStream, const SocketAddress& InFromAddress);

	void UpdateSayingHello();
	void UpdateSendingInputPackets();

	void SendHelloPacket();
	void SendInputPacket();

	void HandleWelcomePacket(InputMemoryBitStream& InInputStream);
	void HandleStatePacket(InputMemoryBitStream& InInputStream);

private:
	NetworkClientState mState = NCS_Uninitialized;
	float mTimeOfLastHello = 0;
	float mTimeOfLastInputPacket = 0;

	const float kTimeBetweenHellos = 1.f;
	const float kTimeBetweenInputPackets = 0.033f;

	SocketAddress mServerAddress;
	string mName;
	int32_t mPlayerId;
};
