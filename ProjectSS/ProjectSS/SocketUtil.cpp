#include "stdafx.h"

void SocketUtil::StartUsingSocket()
{
	//1. 소켓 라이브러리 활성화 (명시적으로 초기화&마무리해줘야 하며, 라이브러리 버전도 설정해야 함)
	WSADATA WsaData;
	int ErrorCode = WSAStartup(MAKEWORD(2, 2), &WsaData); //주버전번호 & 부버전번호, WSAStartup()함수가 활성화된 라이브러리에 대한 정보로 값을 채워줌.
														  //성공시 0 리턴, 실패시 에러코드 리턴
	
	if (ErrorCode != 0)
	{
		ReportError("SocketUtil::StartUsingSocket");
	}
}

void SocketUtil::EndUsingSocket()
{
	//2. 소켓 라이브러리 사용 종료
	int ErrorCode = WSACleanup(); //리턴값 : 에러코드
								  //소켓 동작 강제종료, 리소스 모두 소멸.
								  //따라서 호출 전 모든 소켓이 닫혔고 사용이 끝나는지 확실히 해야함
								  //WSAStartup을 호출한 횟수만큼 Cleanup도 호출해줘야 함 (레퍼런스 카운트 되므로)

	if (ErrorCode != 0)
	{
		ReportError("SocketUtil::EndUsingSocket");
	}
}

UDPSocketPtr SocketUtil::CreateUDPSocket(SocketAddressFamily inFamily)
{
	//소켓이 유효할 때만 생성하도록 함.
	//->UDPSocket이 존재한다면, mSocket은 반드시 살아있음.
	SOCKET s = socket(static_cast<int>(inFamily), SOCK_DGRAM, IPPROTO_UDP);
	if (s != INVALID_SOCKET)
	{
		return UDPSocketPtr(new UDPSocket(s));
	}

	ReportError("SocketUtil::CreateUDPSocket");
	return nullptr;
}


TCPSocketPtr SocketUtil::CreateTCPSocket(SocketAddressFamily inFamily)
{
	//소켓이 유효할 때만 생성하도록 함.
	//->TCPSocket이 존재한다면, mSocket은 반드시 살아있음.
	SOCKET s = socket(static_cast<int>(inFamily), SOCK_STREAM, IPPROTO_TCP);
	if (s != INVALID_SOCKET)
	{
		return TCPSocketPtr(new TCPSocket(s));
	}

	ReportError("SocketUtil::CreateTCPSocket");
	return nullptr;
}

fd_set* SocketUtil::FillSetFromVector(fd_set& OutSet, const vector<TCPSocketPtr>* InSockets)
{
	//TCP 소켓 포인터 벡터로 fd_set을 만들어서 리턴함

	if (InSockets)
	{
		//fd_set을 초기화
		FD_ZERO(&OutSet);

		//fd_set 집합에 인자로 받아온 소켓들을 추가
		for (const TCPSocketPtr& Socket : *InSockets)
		{
			FD_SET(Socket->mSocket, &OutSet);
		}
		return &OutSet;
	}
	
	else return nullptr;
}

void SocketUtil::FillVectorFromSet(vector<TCPSocketPtr>* OutSockets, const vector<TCPSocketPtr>* InSockets, const fd_set& InSet)
{
	//Set에 속하는 TCP 소켓 포인터(InSockets)로 벡터를 만들어 리턴

	if (InSockets && OutSockets)
	{
		OutSockets->clear();
		for(const TCPSocketPtr& Socket : *InSockets)
		{
			//fd_set 집합에 특정 소켓이 포함되었는지 확인함
			if (FD_ISSET(Socket->mSocket, &InSet))
			{
				OutSockets->push_back(Socket);
			}
		}
	}
}

int SocketUtil::Select(const vector<TCPSocketPtr>* InReadSet, vector<TCPSocketPtr>* OutReadSet, const vector<TCPSocketPtr>* InWriteSet, vector<TCPSocketPtr>* OutWriteSet, const vector<TCPSocketPtr>* InExceptSet, vector<TCPSocketPtr>* OutExceptSet)
{
	//여러 소켓을 한꺼번에 확인하고, 그 중 하나라도 준비되면 즉시 대응하는 방법
	// -> select() 함수

	//인자는
	//(1) 읽기용으로 체크할 소켓 / (2) 실제로 읽어올 수 있는 소켓
	//(3) 쓰기용으로 체크할 소켓 / (4) 실제로 쓸 수 있는 소켓
	//(5) 에러를 점검할 소켓	   / (6) 실제로 에러가 발생한 소켓
	//체크 안하고자 하는 부분은 nullptr를 넣어주며, 그러면 fd_set도 null이 된다.
	//인자로 들어은 벡터들의 값은 변경하지 않는다.

	//받은 벡터로 fd_set을 채움
	fd_set Read, Write, Except;
	fd_set* ReadPtr = FillSetFromVector(Read, InReadSet);
	fd_set* WritePtr = FillSetFromVector(Write, InWriteSet);
	fd_set* ExceptPtr = FillSetFromVector(Except, InExceptSet);

	int ToRet = select(0, ReadPtr, WritePtr, ExceptPtr, nullptr);
	//인자는 (1)소켓 식별자이나 윈도에서 의미없음, (2)읽고 싶은 소켓 (3) 쓰기용으로 체크하려는 소켓 
	//		 (4)에러 체크하고싶은 소켓 (5) 타임아웃값, 시간초과시 모든 집합내용 clear되고 select()는 호출스레드로 리턴함 (nullptr은 시간제한X)
	//(2),(3),(4)에 남은 소켓이 있다면, 읽을 수 있고, 쓸 수 있고, 에러가 발생한 소켓들이 남은 것.
	//리턴값은 그 수들의 합.

	if (ToRet > 0)
	{
		FillVectorFromSet(OutReadSet, InReadSet, Read);
		FillVectorFromSet(OutWriteSet, InWriteSet, Write);
		FillVectorFromSet(OutExceptSet, InExceptSet, Except);
	}

	return ToRet;
}

void SocketUtil::ReportError(const char* inOperationDesc)
{
	LPVOID lpMsgBuf;
	DWORD errorNum = GetLastError();

	//DWORD로 넘어온 에러번호를 문자열로 바꿔줌.
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	LOG("Error %s: %d- %s", inOperationDesc, errorNum, lpMsgBuf);
}

int SocketUtil::GetLastError()
{
	return WSAGetLastError();
}
