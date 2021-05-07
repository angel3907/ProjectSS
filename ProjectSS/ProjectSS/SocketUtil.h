#pragma once
#include "UDPSocket.h"
#include "TCPSocket.h"
#include "Player.h"
#include <vector>
#include "MemoryBitStream.h"
#include "MemberVariable.h"
#include "ReplicationHeader.h"

class RPCParams;

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

	static fd_set* FillSetFromVector(fd_set& OutSet, const vector<TCPSocketPtr>* InSockets);
	static void FillVectorFromSet(vector<TCPSocketPtr>* OutSockets, const vector<TCPSocketPtr>* InSockets, const fd_set& InSet);

	static int Select(const vector<TCPSocketPtr>* InReadSet, vector<TCPSocketPtr>* OutReadSet, 
			   const vector<TCPSocketPtr>* InWriteSet, vector<TCPSocketPtr>* OutWriteSet,
			   const vector<TCPSocketPtr>* InExceptSet, vector<TCPSocketPtr>* OutExceptSet);

	static void	ReportError(const char* inOperationDesc);
	static int GetLastError();

	static void SendPlayer(UDPSocketPtr Socket, SocketAddress* ToAddress, const Player* InPlayer);
	static void SendPlayer(TCPSocketPtr Socket, const Player* InPlayer);
	static void ReceivePlayer(UDPSocketPtr Socket, Player* OutPlayer);

	static uint32_t SendPlayerWithBitStream(UDPSocketPtr Socket, SocketAddress& ToAddress, const Player* InPlayer);
	static uint32_t ReceivePlayerWithBitStream(UDPSocketPtr Socket, Player* OutPlayer);

	static uint32_t SendPODWithBitStream(UDPSocketPtr Socket, SocketAddress& ToAddress, const DataType* InDataType, uint8_t* InData);
	static uint32_t ReceivePODWithBitStream(UDPSocketPtr Socket, const DataType* InDataType, uint8_t* OutData);


	static uint32_t SendPacket(UDPSocketPtr Socket, SocketAddress& ToAddress, const vector<GameObject*>& InGameObjects);
	static uint32_t ReceivePacket(UDPSocketPtr Socket);

	//RA_PRC면 InGameObject가 null, RPCParam 존재 / RA_RMI면 InGameObject가 존재, RPCParam 존재 / 그 외에는 RPCParams만 null
	static uint32_t SendReplicated(UDPSocketPtr Socket, SocketAddress& ToAddress, ReplicationAction InReplicationAction, GameObject* InGameObject, RPCParams* InRPCParams);
	static uint32_t ReceiveReplicated(UDPSocketPtr Socket);

	static void Write(OutputMemoryBitStream* InMemoryBitStream, const DataType* InDataType, uint8_t* InData);
	static void Read(InputMemoryBitStream* InMemoryBitStream, const DataType* InDataType, uint8_t* OutData);

	void WriteClassType(OutputMemoryBitStream& InStream, const GameObject* InGameObject);
	GameObject* CreateGameObjectFromStream(InputMemoryBitStream& InStream);
};
