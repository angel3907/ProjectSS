#include "stdafx.h"

UDPSocket::~UDPSocket()
{
	closesocket(mSocket); //������ �ݾ���
}

int UDPSocket::Bind(const SocketAddress& InBindAddress)
{
	int err = bind(mSocket, &InBindAddress.mSockAddr, InBindAddress.GetSize());
	if (err == 0)
	{
		return NO_ERROR;
	}

	//���� ���� �뵵 Ŭ����. ���� �θ� ����ó������� �����ϱ� ������.
	SocketUtil::ReportError("UDPSocket::Bind");
	return SocketUtil::GetLastError();
}

int UDPSocket::SendTo(const void* InData, int InLen, const SocketAddress& InToAddress)
{
	int byteSentCount = sendto(mSocket, static_cast<const char*>(InData), InLen, 0, &InToAddress.mSockAddr, InToAddress.GetSize());
	//���ڴ� (1)���� / (2)�۽� ����(���� ��) / (3)�۽Ź��� ũ�� / (4)�÷��� / (5)���� �ּ� / (6)���� �ּ� ���� 
	//(2) �۽� ���۴� char*�� ��ȯ�� �� ������ � �����͵� ���� �� ����.
	//(3) ������ ���̴� UDP ����� ������ 65535 ����Ʈ
	//	  ������ �п����� ������ �ִ°� �̴��� ���� MTU ����(1500)��ŭ. ���� ���� ����� ��Ŷ ���۸� �����Ѵٸ� 1300����Ʈ ���ϰ� ����.
	//(4) �÷��״� ������ ���� ���� ��Ʈ �÷���

	//������ �۽Ŵ�⿭�� ���� ������ ���̸� ����. �̿� -1 ����
	//������̶�� ������ ������ �Ϸ�Ȱ� �ƴ϶�, ���� ��⿭�� ���� �� ��ϵ� ��.
	if (byteSentCount > 0)
	{
		return byteSentCount;
	}

	//�����ڵ带 ������ ����
	SocketUtil::ReportError("UDPSocket::SendTo");
	return -SocketUtil::GetLastError();
}

int UDPSocket::ReceiveFrom(void* InBuffer, int InMaxLength, SocketAddress& outFromAddress)
{
	int fromLength = outFromAddress.GetSize();
	int byteReadCount = recvfrom(mSocket, static_cast<char*>(InBuffer), InMaxLength, 0, &outFromAddress.mSockAddr, &fromLength);
	//���ڴ� (1)���� / (2)���� ����(���� ��) / (3)���Ź��� ũ�� / (4)�÷��� / (5)�޾ƿ� �ּ� / (6)�޾ƿ� �ּ� ����
	//(1) ������ ���ٸ� �ɼ� �̼�����, recvfrom�� ȣ���ϸ� ���Ͽ� ���� ���ŵ� �����ͱ׷��� ������ �����尡 ����ŷ�Ǿ� ���� ���
	//(2) ���Ź��۴� ���� �纻�� �������� ����
	//(3) ���Ź��� ũ�� �̻��� �����÷ι����� ���� ���� ����. �������� �������Ƿ� �˳��� ���
	//(4) �÷��״� ������ ���� ���� ��Ʈ �÷���. 0�̸� ����ϳ� MSG_PEEK ���� �÷��װ� ����
	//(5) �޾ƿ� �ּҴ� ä��� ���� �ƴ϶� �ʱ�ȭ�Ǵ� ����. ��𿡼� �޾ƿԴ��� Ȯ���ϴ� �뵵
	
	//������ ���Ź��ۿ� ������ ����Ʈ ���̸� ����, �̿� -1 ����
	if (byteReadCount >= 0)
	{
		return byteReadCount;
	}
	else
	{
		int ErrorCode = SocketUtil::GetLastError();

		//������ŷ ����� ���ŵ� �����Ͱ� ������ ����� ��. ���� ������ �ƴϴ� �ٸ��� ó��
		if (ErrorCode == WSAEWOULDBLOCK)
		{
			return 0;
		}
		else
		{
			//�����ڵ带 ������ ����
			SocketUtil::ReportError("UDPSocket::ReceiveFrom");
			return -ErrorCode;
		}
	}
}

int UDPSocket::SetNonBlockingMode(bool InShouldBeNonBlocking)
{
	u_long arg = InShouldBeNonBlocking ? 1 : 0; //1: ������ŷ(0 �̿��� ����), 0 : ����ŷ
	//������ ������ŷ ���� �����
	int result = ioctlsocket(mSocket, FIONBIO, &arg);
	//���ڴ� (1) ������ŷ�ϰ��� �ϴ� ���� (2)�����Ϸ��� ���� �Ķ���� (3)�Ķ���Ϳ� �����Ϸ��� ��. 0 �ܸ̿� ������ŷ.

	if (result != SOCKET_ERROR)
	{
		return NO_ERROR;
	}

	SocketUtil::ReportError("UDPSocket::SetNonBlockingMode");
	return SocketUtil::GetLastError();
}
