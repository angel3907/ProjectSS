#pragma once
#include <string>
#include "NetworkManager.h"
#include "SocketAddress.h"
#include "ReplicationManager.h"
#include "Player.h"
#include "DeliveryNotificationManager.h"

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
	static NetworkManagerClient* sInstance;
	virtual ~NetworkManagerClient() {}

	//네트워크 매니저 초기화	
	static void StaticInit (const SocketAddress& InServerAddress, const std::string& InName);

	//패킷 전송
	void SendOutgoingPackets();

	//패킷 처리
	virtual void ProcessPacket(InputMemoryBitStream& InInputStream, const SocketAddress& InFromAddress) override;

	//플레이어 아이디 얻어오기
	int32_t GetPlayerId() const { return mPlayerId; }

	//서버 소켓 주소 세팅, 플레이어 이름 세팅, 색깔 세팅
	void SetServerSocketAddress(const SocketAddress& InServerAddress) {mServerAddress = InServerAddress;}
	void SetPlayerName(std::string InName){ mName = InName; }
	void SetPlayerColor(PlayerColor InPlayerColor) { mPlayerColor  = InPlayerColor;}

	void SendReadyPacket(ReadyPacketType InReadyPacketType);

private:
	NetworkManagerClient();
	void Init (const SocketAddress& InServerAddress, const std::string& InName); //실제 초기화 함수

	//헬로 패킷 / 입력 패킷 전송
	void UpdateSayingHello();
	void UpdateSendingInputPackets();

	void SendHelloPacket();
	void SendInputPacket();

	//웰컴 패킷 / 상태 (리플리케이션) 패킷 처리
	void HandleWelcomePacket(InputMemoryBitStream& InInputStream);
	void HandleNoAdmittancePacket(InputMemoryBitStream& InInputStream);
	void HandleStatePacket(InputMemoryBitStream& InInputStream);
	void HandleReadyPacket(InputMemoryBitStream& InInputStream);

private:
	//배달 통지 관리자
	DeliveryNotificationManager	mDeliveryNotificationManager;
	//전용 리플리케이션 매니저
	ReplicationManager mReplicationManagerClient;
	
	//헬로 / 입력 패킷 보내는 주기 조정
	float mTimeOfLastHello = 0;
	float mTimeOfLastInputPacket = 0;
	const float kTimeBetweenHellos = 1.f;
	const float kTimeBetweenInputPackets = 0.033f;

	//클라이언트 상태
	NetworkClientState mState = NCS_Uninitialized;

	//서버 주소
	SocketAddress mServerAddress;

	//플레이어 이름과 아이디 색깔
	std::string mName;
	int32_t mPlayerId;
	PlayerColor mPlayerColor;
};
