#include "TCPSocket.h"
#include "stdafx.h"

TCPSocket::~TCPSocket()
{
	closesocket(mSocket);
}

int TCPSocket::Connect(const SocketAddress& InAddress)
{
	//클라가 서버에게 연결 요청
	//호출시 최초 SYN패킷을 대상 호스트에 전송해서 TCP 핸드셰이킹 개시
	//옵션 지정하지 않으면 기본적으로 블로킹
	int Err = connect(mSocket, &InAddress.mSockAddr, InAddress.GetSize());
	//인자는 (1)소켓, (2)연결하려는 호스트주소 (3)주소 길이

	if (Err >= 0) //성공시 0, 실패시 -1
	{
		return NO_ERROR;
	}

	SocketUtil::ReportError("TCPSocket::Connect");
	return -SocketUtil::GetLastError();
}

int TCPSocket::Bind(const SocketAddress& InToAddress)
{
	int Err = bind(mSocket, &InToAddress.mSockAddr, InToAddress.GetSize());
	
	if (Err >= 0) //성공시 0, 실패시 -1
	{
		return NO_ERROR;
	}
	SocketUtil::ReportError("TCPSocket::Bind");
	return -SocketUtil::GetLastError();
}

int TCPSocket::Listen(int InBackLog /*= 32*/)
{
	//서버에서 클라의 핸드셰이킹을 듣기 시작
	//TCP에서 클라-서버간 연결 초기 수립을 위해선 3-way handshaking이 필요
	//핸드셰이킹 첫단계를 받으려면
	//1)소켓 만들기 - 2)소켓을 특정 포트에 바인딩 - 3)listen
	int Err = listen(mSocket, InBackLog);
	//인자는 (1)리스닝 모드에 둘 소켓, (2)들어올 연결을 대기열에 둘 최대 숫자를 지정 (기본값은 SOMAXCONN)
	//(1) 소켓이 리스닝 모드면, 외부에서 들어오는 요청을 받아 대기열에 저장함. accept 호출시 연결요청의 다음 핸드셰이킹 수행
	
	if (Err >= 0) //성공시 0, 실패시 -1
	{
		return NO_ERROR;
	}
	SocketUtil::ReportError("TCPSocket::Listen");
	return -SocketUtil::GetLastError();
}

TCPSocketPtr TCPSocket::Accept(SocketAddress& InFromAddress)
{
	//서버가 들어오는 연결을 받아 계속 진행하려면 accept 호출
	int length = InFromAddress.GetSize();
	SOCKET newSocket = accept(mSocket, &InFromAddress.mSockAddr, &length);
	//인자는 (1)리스닝소켓 (2)연결요청하는 원격호스트의 주소로 채워짐 (3)Addr 버퍼의 포인터길이
	//성공시 새 소켓을 리턴, 이로 원격 호스트와 통신 가능 (리스닝소켓과 같은포트)
	//TCP는 연결된 각 원격 호스트마다 소켓을 하나씩 갖고있어야 함.
	//이 소켓에는 원격 호스트 주소, 포트, 호스트로 보내는 패킷 전부가 저장됨(재전송 용도).
	//받을 게 없으면 블로킹됨

	//유효한 소켓이 생성되었을 때만 포인터를 리턴
	if (newSocket != INVALID_SOCKET)
	{
		return TCPSocketPtr(new TCPSocket(newSocket)); //더이상 소켓이 참조되는 곳이 없을 때 자동으로 닫힘.
	}
	SocketUtil::ReportError("TCPSocket::Accept");
	return nullptr;
}

int TCPSocket::Send(const void* InData, int InLen)
{
	//TCP 소켓은 보낼 주소를 갖고있어 매번 넘겨주지 않아도 됨.
	//데이터 전송 함수
	int byteSentCount = send(mSocket, static_cast<const char*>(InData), InLen, 0);
	//인자는 (1)소켓 (2)송신버퍼 (3)송신할 데이터크기 (4) 데이터전송 제어플래그
	//(2) UDP와 달리 버퍼가 데이터그램이 아니고 한번에 전송된다는 보장이 X.
	//	  대신 데이터는 소켓의 외부전송용 버퍼에 추가되었다가, 소켓라이브러리에 의해 적당한 때에 전송됨.
	//(3) UDP와는 달리 MTU보다 작게 잡을 필요 X. 소켓 전송버퍼에 자리가있다면 라이브러리는 데이터 모두 전송가능.
	//	  이를 위해 데이터를 적당히 잘라서 보낼 것임.

	//성공시 전송한 데이터 길이를 리턴함. len보다 작다면 전송버퍼가 모자라서 여유공간만큼만 보낼것임
	//공간이 아예 없으면 호출스레드는 블로킹되서 공간생길때까지 기다림.
	//에러시 -1 리턴, 리턴값 중 양수값은 전송완료가 아니라 전송대기열에 등록된 것.
	if (byteSentCount >= 0)
	{
		return byteSentCount;
	}
	SocketUtil::ReportError("TCPSocket::Send");
	return -SocketUtil::GetLastError();
}

int TCPSocket::Receive(void* InBuffer, int InLen)
{
	//데이터 수신 함수
	int byteReceivedCount = recv(mSocket, static_cast<char*>(InBuffer), InLen, 0);
	//(1) 소켓 (2)수신버퍼 (3) 최대로받을 데이터크기 (4) 비트플래그
	//(2) 버퍼에 데이터를 복사하고나면 해당 데이터가 소켓내부 수신버퍼에서 제거됨.

	//성공시 수신한 바이트길이 리턴. len 이하값.
	//send 한번 호출해서 일정길이 바이트를 보냈을 때, 상대가 recv를 호출했다고 같은 길이를 받는다고 보장불가.
	//왜냐면 보내는측의 네트웤 라이브러리가 모았다가 보낼수도 있어서.
	//에러시 -1리턴, 수신버퍼에 데이터가 없으면 블로킹됨.
	if (byteReceivedCount >= 0)
	{
		return byteReceivedCount;
	}

	SocketUtil::ReportError("TCPSocket::Receive");
	return -SocketUtil::GetLastError();
}
