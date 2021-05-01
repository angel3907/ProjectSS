#include "TCPSocket.h"
#include "stdafx.h"

TCPSocket::~TCPSocket()
{
	closesocket(mSocket);
}

int TCPSocket::Connect(const SocketAddress& InAddress)
{
	//Ŭ�� �������� ���� ��û
	//ȣ��� ���� SYN��Ŷ�� ��� ȣ��Ʈ�� �����ؼ� TCP �ڵ����ŷ ����
	//�ɼ� �������� ������ �⺻������ ���ŷ
	int Err = connect(mSocket, &InAddress.mSockAddr, InAddress.GetSize());
	//���ڴ� (1)����, (2)�����Ϸ��� ȣ��Ʈ�ּ� (3)�ּ� ����

	if (Err >= 0) //������ 0, ���н� -1
	{
		return NO_ERROR;
	}

	SocketUtil::ReportError("TCPSocket::Connect");
	return -SocketUtil::GetLastError();
}

int TCPSocket::Bind(const SocketAddress& InToAddress)
{
	int Err = bind(mSocket, &InToAddress.mSockAddr, InToAddress.GetSize());
	
	if (Err >= 0) //������ 0, ���н� -1
	{
		return NO_ERROR;
	}
	SocketUtil::ReportError("TCPSocket::Bind");
	return -SocketUtil::GetLastError();
}

int TCPSocket::Listen(int InBackLog /*= 32*/)
{
	//�������� Ŭ���� �ڵ����ŷ�� ��� ����
	//TCP���� Ŭ��-������ ���� �ʱ� ������ ���ؼ� 3-way handshaking�� �ʿ�
	//�ڵ����ŷ ù�ܰ踦 ��������
	//1)���� ����� - 2)������ Ư�� ��Ʈ�� ���ε� - 3)listen
	int Err = listen(mSocket, InBackLog);
	//���ڴ� (1)������ ��忡 �� ����, (2)���� ������ ��⿭�� �� �ִ� ���ڸ� ���� (�⺻���� SOMAXCONN)
	//(1) ������ ������ ����, �ܺο��� ������ ��û�� �޾� ��⿭�� ������. accept ȣ��� �����û�� ���� �ڵ����ŷ ����
	
	if (Err >= 0) //������ 0, ���н� -1
	{
		return NO_ERROR;
	}
	SocketUtil::ReportError("TCPSocket::Listen");
	return -SocketUtil::GetLastError();
}

TCPSocketPtr TCPSocket::Accept(SocketAddress& InFromAddress)
{
	//������ ������ ������ �޾� ��� �����Ϸ��� accept ȣ��
	int length = InFromAddress.GetSize();
	SOCKET newSocket = accept(mSocket, &InFromAddress.mSockAddr, &length);
	//���ڴ� (1)�����׼��� (2)�����û�ϴ� ����ȣ��Ʈ�� �ּҷ� ä���� (3)Addr ������ �����ͱ���
	//������ �� ������ ����, �̷� ���� ȣ��Ʈ�� ��� ���� (�����׼��ϰ� ������Ʈ)
	//TCP�� ����� �� ���� ȣ��Ʈ���� ������ �ϳ��� �����־�� ��.
	//�� ���Ͽ��� ���� ȣ��Ʈ �ּ�, ��Ʈ, ȣ��Ʈ�� ������ ��Ŷ ���ΰ� �����(������ �뵵).
	//���� �� ������ ���ŷ��

	//��ȿ�� ������ �����Ǿ��� ���� �����͸� ����
	if (newSocket != INVALID_SOCKET)
	{
		return TCPSocketPtr(new TCPSocket(newSocket)); //���̻� ������ �����Ǵ� ���� ���� �� �ڵ����� ����.
	}
	SocketUtil::ReportError("TCPSocket::Accept");
	return nullptr;
}

int TCPSocket::Send(const void* InData, int InLen)
{
	//TCP ������ ���� �ּҸ� �����־� �Ź� �Ѱ����� �ʾƵ� ��.
	//������ ���� �Լ�
	int byteSentCount = send(mSocket, static_cast<const char*>(InData), InLen, 0);
	//���ڴ� (1)���� (2)�۽Ź��� (3)�۽��� ������ũ�� (4) ���������� �����÷���
	//(2) UDP�� �޸� ���۰� �����ͱ׷��� �ƴϰ� �ѹ��� ���۵ȴٴ� ������ X.
	//	  ��� �����ʹ� ������ �ܺ����ۿ� ���ۿ� �߰��Ǿ��ٰ�, ���϶��̺귯���� ���� ������ ���� ���۵�.
	//(3) UDP�ʹ� �޸� MTU���� �۰� ���� �ʿ� X. ���� ���۹��ۿ� �ڸ����ִٸ� ���̺귯���� ������ ��� ���۰���.
	//	  �̸� ���� �����͸� ������ �߶� ���� ����.

	//������ ������ ������ ���̸� ������. len���� �۴ٸ� ���۹��۰� ���ڶ� ����������ŭ�� ��������
	//������ �ƿ� ������ ȣ�⽺����� ���ŷ�Ǽ� �������涧���� ��ٸ�.
	//������ -1 ����, ���ϰ� �� ������� ���ۿϷᰡ �ƴ϶� ���۴�⿭�� ��ϵ� ��.
	if (byteSentCount >= 0)
	{
		return byteSentCount;
	}
	SocketUtil::ReportError("TCPSocket::Send");
	return -SocketUtil::GetLastError();
}

int TCPSocket::Receive(void* InBuffer, int InLen)
{
	//������ ���� �Լ�
	int byteReceivedCount = recv(mSocket, static_cast<char*>(InBuffer), InLen, 0);
	//(1) ���� (2)���Ź��� (3) �ִ�ι��� ������ũ�� (4) ��Ʈ�÷���
	//(2) ���ۿ� �����͸� �����ϰ��� �ش� �����Ͱ� ���ϳ��� ���Ź��ۿ��� ���ŵ�.

	//������ ������ ����Ʈ���� ����. len ���ϰ�.
	//send �ѹ� ȣ���ؼ� �������� ����Ʈ�� ������ ��, ��밡 recv�� ȣ���ߴٰ� ���� ���̸� �޴´ٰ� ����Ұ�.
	//�ֳĸ� ���������� ��Ʈ�p ���̺귯���� ��Ҵٰ� �������� �־.
	//������ -1����, ���Ź��ۿ� �����Ͱ� ������ ���ŷ��.
	if (byteReceivedCount >= 0)
	{
		return byteReceivedCount;
	}

	SocketUtil::ReportError("TCPSocket::Receive");
	return -SocketUtil::GetLastError();
}
