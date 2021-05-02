#include "stdafx.h"

#define GOOD_SEGMENT_SIZE 1500
bool bGameRunning = true;

void DoTCPLoop()
{
	//TCP 서버를 개설하고 리스닝
	TCPSocketPtr ListenSocket = SocketUtil::CreateTCPSocket(INET);
	SocketAddress ReceivingAddress(INADDR_ANY, 48000);
	
	if (ListenSocket->Bind(ReceivingAddress) != NO_ERROR)
	{
		return;	
	}

	vector<TCPSocketPtr> ReadBlockSockets; //읽을 소켓
	ReadBlockSockets.push_back(ListenSocket);
	vector<TCPSocketPtr> ReadableSockets;

	while (bGameRunning)
	{
		//-> Select에서 ReadBlockSockets의 소켓들 중 하나가 패킷을 수신할 때까지 리턴하지 않음 (블로킹)
		if (!SocketUtil::Select(&ReadBlockSockets, &ReadableSockets, nullptr, nullptr, nullptr, nullptr))
		{
			continue;
		}
		
		// ReadableSockets에 받은 패킷을 하나씩 순회한다.
		for (const TCPSocketPtr& Socket : ReadableSockets)
		{
			//리스닝 소켓에서 직접 데이터를 읽진 않으나, select()에 리스닝 중인 소켓을 읽기용으로 넘기면
			//신규 접속이 새로 들어오면 "읽을 것이 있음" 으로 처리됨.
			if (Socket == ListenSocket) 
			{
				//리스닝 소켓에서 새 연결을 받음
				//해당 소켓을 다시 읽을 소켓 목록에 넣음
				SocketAddress NewClientAddress;
				auto NewSocket = ListenSocket->Accept(NewClientAddress);
				ReadBlockSockets.push_back(NewSocket);

				//새 클라이언트가 접속하였음을 게임에 통지
				//ProcessNewClient(NewSocket, NewClientAddress);
			}
			else
			{
				//일반 소켓이므로 데이터를 수신함
				char Segment[GOOD_SEGMENT_SIZE];
				int DataReceived = Socket->Receive(Segment, GOOD_SEGMENT_SIZE);
				if (DataReceived > 0)
				{
					//클라이언트로부터의 데이터를 받아서 처리함
					//ProcessDataFromClient(Socket, Segment, DataReceived);
				}
			}
		}

	}
}


void DoGameFrame()
{

}

void DoGameLoop()
{
	UDPSocketPtr MySock = SocketUtil::CreateUDPSocket(INET);
	MySock->SetNonBlockingMode(true);
	MySock->Bind(SocketAddress()); //이후에 추가.. 이런 실수를..

	while (bGameRunning)
	{
		char data[1500];
		SocketAddress socketAddress;

		//논블로킹 모드로 소켓을 설정하면 수신된 데이터가 없어도 바로 리턴된다.
		int ByteReceived = MySock->ReceiveFrom(data, sizeof(data), socketAddress);
		if (ByteReceived > 0)
		{
			//매 프레임마다 받은 데이터 있는지 체크해 처리.
			//만일 한번에 여러 데이터그램을 처리하고 싶으면 루프문을 하나 더 추가해
			//최대값으로 지정한 숫자 or 전체를 처리할 때까지 루프를 돌려 읽어들이면 됨.
			//단 프레임당 처리할 데이터그램 개수엔 반드시 제약을 둘 것 (악성클라가 데이터그램을 쏟아부으면 시뮬레이션 못하게 될 수도)
			//ProcessReceivedData(data, ByteReceived, socketAddress);
		}

		DoGameFrame();
	}
}

int main()
{
	//1. 소켓 라이브러리 활성화 (명시적으로 초기화&마무리해줘야 하며, 라이브러리 버전도 설정해야 함)
	WSADATA WsaData;
	WSAStartup(MAKEWORD(2, 2), &WsaData); //주버전번호 & 부버전번호, WSAStartup()함수가 활성화된 라이브러리에 대한 정보로 값을 채워줌.
										  //성공시 0 리턴, 실패시 에러코드 리턴

	//DoGameLoop();
	DoTCPLoop();

	//2. 소켓 라이브러리 사용 종료
	int ErrorCode = WSACleanup(); //리턴값 : 에러코드
								  //소켓 동작 강제종료, 리소스 모두 소멸.
								  //따라서 호출 전 모든 소켓이 닫혔고 사용이 끝나는지 확실히 해야함
								  //WSAStartup을 호출한 횟수만큼 Cleanup도 호출해줘야 함 (레퍼런스 카운트 되므로)

	if (ErrorCode == SOCKET_ERROR) //-1
	{
		printf("socket error!");
		int ErrorCodeDetail = WSAGetLastError(); //소켓 라이브러리 사용중 마지막으로 발생한 에러를 가져옴.
												 //소켓 라이브러리 함수 결과로 -1을 받았다면 바로 불러야 에러 이유를 알 수 있음
	}
}