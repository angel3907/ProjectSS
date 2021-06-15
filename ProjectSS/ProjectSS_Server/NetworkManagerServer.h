#pragma once
#include "NetworkManager.h"
#include "InputMemoryBitStream.h"
#include "SocketAddress.h"
#include <unordered_map>
#include "ClientProxy.h"
#include "ReplicationManagerTransmissionData.h"

class NetworkManagerServer : public NetworkManager
{
public:
	static NetworkManagerServer* sInstance;
	virtual ~NetworkManagerServer(){}

	//��Ʈ��ũ �Ŵ��� �ʱ�ȭ
	static bool StaticInit(uint16_t InPort);

	//��Ŷ �۽�
	void SendOutgoingPackets();
	
	//��Ŷ ó��
	virtual void ProcessPacket(InputMemoryBitStream& InInputStream, const SocketAddress& InFromAddress) override;

	//Ư�� Ŭ���̾�Ʈ ���Ͻ� ������ ��������
	ClientProxyPtr GetClientProxy (int PlayerId) const;

	//��� Ŭ���̾�Ʈ ������Ʈ
	void UpdateAllClients();

	//��� Ŭ���̾�Ʈ�� ���ø����̼� ��Ŷ �߰�
	void AddUnprocessedRAToAllClients(ReplicationCommand& RA);

	//��� Ŭ���̾�Ʈ���� ���ø����̼� ��Ŷ ����
	void SendReplicatedToAllClients(ReplicationAction InReplicationAction, GameObject* InGameObject, RPCParams* InRPCParams);

	//���� Ŭ���̾�Ʈ ó��
	void HandleLostClient(ClientProxyPtr InClientProxy);

	//���� ���� ó��
	virtual void HandleConnectionReset(const SocketAddress& InFromAddress) override;

	//���� ���� üũ
	void CheckForDisconnects();

	bool IsAllPlayersReady();

	//�غ� ��Ŷ ����
	void SendReadyPacket(ClientProxyPtr InClientProxy, ReadyPacketType InReadyPacketType);

	//�غ� ��Ŷ�� ��ο��� ����
	void SendReadyPacketToAllClient(ReadyPacketType InReadyPacketType);

	//���� ����� Ŭ���̾�Ʈ���� ���� ���� ó��
	void HandleGameEnd();

private:
	NetworkManagerServer();

	//���ø����̼� ��Ŷ ����
	size_t SendReplicated(const SocketAddress& InToAddress, ReplicationManager& InReplicationManager,
		ReplicationManagerTransmissionDataPtr InTransmissionData,
		DeliveryNotificationManager* InDeliveryNotificationManager,
		ReplicationAction InReplicationAction, GameObject* InGameObject, int InNetworkId, RPCParams* InRPCParams);

	//�� Ŭ���̾�Ʈ�κ��� �� ��Ŷ ó�� (��� ��Ŷ)
	void HandlePacketFromNewClient(InputMemoryBitStream& InInputStream, const SocketAddress& InFromAddress);

	//��Ŷ ó�� (�Է� ��Ŷ)
	void ProcessPacket(ClientProxyPtr InClientProxy, InputMemoryBitStream& InInputStream);

	//���� ��Ŷ �۽�
	void SendWelcomePacket(ClientProxyPtr InClientProxy);

	//�غ� ��Ŷ ó��
	void HandleReadyPacket(ClientProxyPtr InClientProxy, InputMemoryBitStream& InInputStream);

	//���� �Ұ� ��Ŷ �۽�
	void SendNoAdmittancePacket(const SocketAddress& InFromAddress, NoAdmittanceReason InReason);

	//���� ��Ŷ �۽�
	void SendStatePacketToClient(ClientProxyPtr InClientProxy);

	//���� �Է� ��Ŷ ó��
	void HandleInputPacket(ClientProxyPtr InClientProxy, InputMemoryBitStream& InInputStream);

	typedef std::unordered_map <int, ClientProxyPtr> IntToClientMap;
	typedef std::unordered_map <SocketAddress, ClientProxyPtr> AddressToClientMap;

	//���� �ּ� - Ŭ���̾�Ʈ ���Ͻ� ��
	AddressToClientMap mAddressToClientMap;

	//�÷��̾� ���̵� - Ŭ���̾�Ʈ ���Ͻ� ��
	IntToClientMap mPlayerIdToClientMap;

	//���� �Ҵ��� �÷��̾� ���̵�
	int mNewPlayerId;

	//���� ��Ŷ ������ �ֱ�
	float mTimeBetweenStatePackets;

	//���� ��Ŷ�� ���������� ���� �ð�
	float mTimeOfLastStatePackets;

	//Ŭ���̾�Ʈ ���� ���� �ð�
	float mClientDisconnectTimeout;
};