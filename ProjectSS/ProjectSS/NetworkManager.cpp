#include "stdafx.h"

void NetworkManager::SendPacket(OutputMemoryBitStream& InOutputStream, SocketAddress& InToAddress)
{
	int32_t SentByteCount = mSocket->SendTo(InOutputStream.GetBufferPtr(), InOutputStream.GetByteLength(), InToAddress);
	//���� ����Ʈ ���� ���� ó���� ���߿�
}