#pragma once
#include <vector>
#include "UDPSocket.h"
#include "TCPSocket.h"
#include "Player.h"
#include "MemoryBitStream.h"
#include "MemberVariable.h"
#include "ReplicationHeader.h"
#include "ReplicationManager.h"

class RPCParams;
using TCPSocketPtr = std::shared_ptr<TCPSocket>;

enum SocketAddressFamily 
{
	INET = AF_INET,
	INET6 = AF_INET6
};

enum class ClassIdentifier : uint8_t
{
	PLAYER,
	STAR
};

class SocketUtil
{
public:
	static void StartUsingSocket();
	static void EndUsingSocket();

	static UDPSocketPtr CreateUDPSocket(SocketAddressFamily inFamily);
	static TCPSocketPtr CreateTCPSocket(SocketAddressFamily inFamily);

	static fd_set* FillSetFromVector(fd_set& OutSet, const std::vector<TCPSocketPtr>* InSockets);
	static void FillVectorFromSet(std::vector<TCPSocketPtr>* OutSockets, const std::vector<TCPSocketPtr>* InSockets, const fd_set& InSet);

	static int Select(const std::vector<TCPSocketPtr>* InReadSet, std::vector<TCPSocketPtr>* OutReadSet, 
			   const std::vector<TCPSocketPtr>* InWriteSet, std::vector<TCPSocketPtr>* OutWriteSet,
			   const std::vector<TCPSocketPtr>* InExceptSet, std::vector<TCPSocketPtr>* OutExceptSet);

	static void	ReportError(const char* inOperationDesc);
	static int GetLastError();

	static void SendPlayer(UDPSocketPtr Socket, SocketAddress* ToAddress, const Player* InPlayer);
	static void SendPlayer(TCPSocketPtr Socket, const Player* InPlayer);
	static void ReceivePlayer(UDPSocketPtr Socket, Player* OutPlayer);

	static size_t SendPlayerWithBitStream(UDPSocketPtr Socket, SocketAddress& ToAddress, const Player* InPlayer);
	static size_t ReceivePlayerWithBitStream(UDPSocketPtr Socket, Player* OutPlayer);

	static size_t SendPODWithBitStream(UDPSocketPtr Socket, SocketAddress& ToAddress, const DataType* InDataType, uint8_t* InData);
	static size_t ReceivePODWithBitStream(UDPSocketPtr Socket, const DataType* InDataType, uint8_t* OutData);


	static size_t SendPacket(UDPSocketPtr Socket, SocketAddress& ToAddress, const std::vector<GameObject*>& InGameObjects, ReplicationManager& InReplicationManager);
	static size_t ReceivePacket(UDPSocketPtr Socket, ReplicationManager& InReplicationManager);

	//RA_PRC면 InGameObject가 null, RPCParam 존재 / RA_RMI면 InGameObject가 존재, RPCParam 존재 / 그 외에는 RPCParams만 null
	static size_t SendReplicated(UDPSocketPtr Socket, SocketAddress& ToAddress, ReplicationAction InReplicationAction, GameObject* InGameObject, RPCParams* InRPCParams, ReplicationManager& InReplicationManager);
	static size_t ReceiveReplicated(UDPSocketPtr Socket, ReplicationManager& InReplicationManager);

	static void Write(OutputMemoryBitStream* InMemoryBitStream, const DataType* InDataType, uint8_t* InData);
	static void Read(InputMemoryBitStream* InMemoryBitStream, const DataType* InDataType, uint8_t* OutData);

	void WriteClassType(OutputMemoryBitStream& InStream, const GameObject* InGameObject);
	GameObject* CreateGameObjectFromStream(InputMemoryBitStream& InStream);
};
