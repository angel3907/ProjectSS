#include "stdafx.h"

bool bGameRunning = true;

void DoGameFrame()
{

}

void DoGameLoop()
{
	UDPSocketPtr MySock = SocketUtil::CreateUDPSocket(INET);
	MySock->SetNonBlockingMode(true);
	MySock->Bind(SocketAddress()); //���⸦ �ּ�ó���ϸ� ���� �߻�

	while (bGameRunning)
	{
		char data[1500];
		SocketAddress socketAddress;

		//������ŷ ���� ������ �����ϸ� ���ŵ� �����Ͱ� ��� �ٷ� ���ϵȴ�.
		int ByteReceived = MySock->ReceiveFrom(data, sizeof(data), socketAddress);
		if (ByteReceived > 0)
		{
			//�� �����Ӹ��� ���� ������ �ִ��� üũ�� ó��.
			//���� �ѹ��� ���� �����ͱ׷��� ó���ϰ� ������ �������� �ϳ� �� �߰���
			//�ִ밪���� ������ ���� or ��ü�� ó���� ������ ������ ���� �о���̸� ��.
			//�� �����Ӵ� ó���� �����ͱ׷� ������ �ݵ�� ������ �� �� (�Ǽ�Ŭ�� �����ͱ׷��� ��ƺ����� �ùķ��̼� ���ϰ� �� ����)
			//ProcessReceivedData(data, ByteReceived, socketAddress);
		}

		DoGameFrame();
	}
}

int main()
{
	//1. ���� ���̺귯�� Ȱ��ȭ (���������� �ʱ�ȭ&����������� �ϸ�, ���̺귯�� ������ �����ؾ� ��)
	WSADATA WsaData;
	WSAStartup(MAKEWORD(2, 2), &WsaData); //�ֹ�����ȣ & �ι�����ȣ, WSAStartup()�Լ��� Ȱ��ȭ�� ���̺귯���� ���� ������ ���� ä����.
										  //������ 0 ����, ���н� �����ڵ� ����

	DoGameLoop();

	//2. ���� ���̺귯�� ��� ����
	int ErrorCode = WSACleanup(); //���ϰ� : �����ڵ�
								  //���� ���� ��������, ���ҽ� ��� �Ҹ�.
								  //���� ȣ�� �� ��� ������ ������ ����� �������� Ȯ���� �ؾ���
								  //WSAStartup�� ȣ���� Ƚ����ŭ Cleanup�� ȣ������� �� (���۷��� ī��Ʈ �ǹǷ�)

	if (ErrorCode == SOCKET_ERROR) //-1
	{
		printf("socket error!");
		int ErrorCodeDetail = WSAGetLastError(); //���� ���̺귯�� ����� ���������� �߻��� ������ ������.
												 //���� ���̺귯�� �Լ� ����� -1�� �޾Ҵٸ� �ٷ� �ҷ��� ���� ������ �� �� ����
	}
/*
	SOCKET UdpSocket = socket(AF_INET, SOCK_DGRAM, 0); //IPV4, �����ͱ׷� ����(for UDP), ���� ������ ���� �������� �ڵ� ����(0)
	SOCKET TcpSocket = socket(AF_INET, SOCK_STREAM, 0); //IPV4, ���׸�Ʈ ����(for TCP), ���� ������ ���� �������� �ڵ� ����(0) 

	//IPV4 ��Ŷ�� �ּ� ����ü - ���� 1
	sockaddr_in SockAddr{};
	SockAddr.sin_family = AF_INET; //�ּ� ������ ��Ÿ���� �����. ���Ͽ� ���� �Ͱ� ���ƾ� ��
	
	//���� �ּ� ����ü���� ���� ����Ʈ�� �� ���ڸ� ȣ��Ʈ ������ �ƴ϶� ��Ʈ��ũ ���� ü��� ��ȯ�ؾ� �Ѵ�.
	//��ȯ�ϴ� �Լ��� htons() htonl() (host to network short(16��Ʈ), host to network long(32��Ʈ))
	SockAddr.sin_port = htons(7000); //��Ʈ��, 2����Ʈ (����� ��Ʈ ������ ���ǰ����� �־�����)
	SockAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//4����Ʈ ipv4 �ּ�. ���Ͽ����� ���ε� ����ü. (iNADDR_ANY : ���� ��ǻ���� �ּ�)
	//���������� sin_zero ����� �ִµ� �̴� �е���. 0���� ä�� ��

	//���� ��Ŷ�� �����ϴ� ��� ���� ���̺귯���� sockaddr_in ����ü ������ ä���ִ� ���쵵 ����.
	//�̶� sockaddr_in�� �� �ʵ�� ��Ʈ��ũ ����Ʈ ������ ä���� ������ �̸� �ùٸ� ������ ����Ϸ���
	//ntohs(), ntohl() (network to host short(16��Ʈ), network to host long(32��Ʈ))�� ȣ���� ��Ʈ��ũ -> ȣ��Ʈ ����Ʈ ������ ��ȯ.

	//���� 2
	sockaddr_in SockAddr2{};
	SockAddr2.sin_family = AF_INET; //�ּ� ������ ��Ÿ���� �����. ���Ͽ� ���� �Ͱ� ���ƾ� ��
	SockAddr.sin_port = htons(80); //��Ʈ��, 2����Ʈ
	SockAddr.sin_addr.S_un.S_un_b.s_b1 = 65; //4����Ʈ ipv4 �ּ�, ����Ʈ ������ ������ ��ȯ���� �ʿ䰡 ����.
	SockAddr.sin_addr.S_un.S_un_b.s_b2 = 254;
	SockAddr.sin_addr.S_un.S_un_b.s_b3 = 248;
	SockAddr.sin_addr.S_un.S_un_b.s_b4 = 180;


	//���� ���ε�
	//-> � ������ Ư�� �ּҿ� ��Ʈ��ȣ�� ���ڴٰ� �ü������ �˷��ִ� ����.
	//���ε��� �ǹ� 2����
	//1) os�� �� �ּ�,��Ʈ�� �������� �߽ŵ� ��Ŷ ���Ž� os�� �� ���Ͽ��� �Ѱ���
	//2) bind���� ������ �ּ�,��Ʈ�� �� ������ �߽��ϴ� ��Ŷ�� ��Ʈ��ũ/���� ���� ��� �߽� �ּ�, ��Ʈ�� ��.

	//�Ϲ������� �ϳ��� �ּ�, ��Ʈ �� �� ���ϸ� ���ε� ����. �̹� ���ε�� �ּҷδ� ���ε忡����.
	//���н� �̻�� ��Ʈ ã�� ������ ���ε� �õ��ؾ� �ϴµ�, �̶� ��Ʈ��ȣ 0���� ���ָ� ���̺귯���� �ڵ����� �̻������ ��Ʈ�ϳ��� ��� ���ε���
	//������ �ۼ����� ���ؼ� �ݵ�� ���ε��� �ʿ�. ���� ���ε� �� �� �������� �����͸� ������ �ϸ� ���̺귯���� �ڵ����� ���� ��Ʈ�� ������ ���ε���.
	//�׷��� Ŭ��� ���ֵ� �Ǵµ�, �������� ��ǥ�� �ּҿ� ��Ʈ�� ��Ŷ�� �޾ƾ� �Ѵٸ� ���ε��� �ʿ�

	sockaddr_in SockAddr3{};
	SockAddr3.sin_family = AF_INET;
	SockAddr3.sin_port = htons(0);
	//ȣ��Ʈ���� �������� NIC�� ���� �� �ְ� ���� IP�� �ٸ�. INADDR_ANY�� ���ָ� ��� NIC�� IP�ּ��� �ش� ��Ʈ�� ���� ���ε��ϰ� ��
	SockAddr3.sin_addr.S_un.S_addr = htonl(INADDR_ANY); 
 
	//bind() �Լ��� ���ڴ� ���ε��� ���� / ���� �ּ� ����ü / ���� �ּ� ����ü ������
	int BindErrorCode = bind(TcpSocket, reinterpret_cast<sockaddr*>(&SockAddr3), sizeof(sockaddr));

	if (BindErrorCode != 0) //���ϰ��� 0�̸� ���� -1�̸� ����.
	{
		printf("Bind Fail!");
	}

	shutdown(TcpSocket, SD_SEND); //�۽� �ߴ�
	shutdown(TcpSocket, SD_RECEIVE); //���� �ߴ�
	shutdown(TcpSocket, SD_BOTH); //�ۼ��� �ߴ� ->�ۼ������� ��� �����͸� �ۼ��� �Ϸ��� FIN ��Ŷ�� ������ ������ �����ϰ� �ݵ��� ��.

	closesocket(UdpSocket); //���� �ݱ� (����� ��ġ�� �ݵ�� �ݴ´�.)
	closesocket(TcpSocket); //������ ������ ���� ���ҽ��� �ü���� �ݳ�
*/
}