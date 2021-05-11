#include "stdafx.h"

void NetworkManager::SendPacket(OutputMemoryBitStream& InOutputStream, SocketAddress& InToAddress)
{
	int32_t SentByteCount = mSocket->SendTo(InOutputStream.GetBufferPtr(), InOutputStream.GetByteLength(), InToAddress);
	//받은 바이트 수에 대한 처리는 나중에
}