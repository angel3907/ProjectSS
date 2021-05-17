#include "stdafx.h"

UDPSocket::~UDPSocket()
{
	closesocket(mSocket); //소켓을 닫아줌
}

int UDPSocket::Bind(const SocketAddress& InBindAddress)
{
	int err = bind(mSocket, &InBindAddress.mSockAddr, InBindAddress.GetSize());
	if (err == 0)
	{
		return NO_ERROR;
	}

	//오류 보고 용도 클래스. 따로 두면 오류처리방식을 조정하기 쉬워짐.
	SocketUtil::ReportError("UDPSocket::Bind");
	return SocketUtil::GetLastError();
}

int UDPSocket::SendTo(const void* InData, int InLen, const SocketAddress& InToAddress)
{
	int byteSentCount = sendto(mSocket, static_cast<const char*>(InData), InLen, 0, &InToAddress.mSockAddr, InToAddress.GetSize());
	//인자는 (1)소켓 / (2)송신 버퍼(보낼 것) / (3)송신버퍼 크기 / (4)플래그 / (5)보낼 주소 / (6)보낼 주소 길이 
	//(2) 송신 버퍼는 char*로 변환할 수 있으면 어떤 데이터든 보낼 수 있음.
	//(3) 데이터 길이는 UDP 헤더를 포함한 65535 바이트
	//      하지만 분열없이 보낼수 있는건 이더넷 계층 MTU 길이(1500)만큼. 여러 계층 헤더와 패킷 래퍼를 포함한다면 1300바이트 이하가 적정.
	//(4) 플래그는 데이터 전송 제어 비트 플래그

	//성공시 송신대기열에 넣은 데이터 길이를 리턴. 이외 -1 리턴
	//양수값이라고 데이터 전송이 완료된게 아니라, 전송 대기열에 이제 막 등록된 것.
	if (byteSentCount > 0)
	{
		return byteSentCount;
	}

	//에러코드를 음수로 리턴
	SocketUtil::ReportError("UDPSocket::SendTo");
	return -SocketUtil::GetLastError();
}

int UDPSocket::ReceiveFrom(void* InBuffer, int InMaxLength, SocketAddress& outFromAddress)
{
	int fromLength = outFromAddress.GetSize();
	int byteReadCount = recvfrom(mSocket, static_cast<char*>(InBuffer), InMaxLength, 0, &outFromAddress.mSockAddr, &fromLength);
	//인자는 (1)소켓 / (2)수신 버퍼(받을 것) / (3)수신버퍼 크기 / (4)플래그 / (5)받아온 주소 / (6)받아온 주소 길이
	//(1) 소켓은 별다른 옵션 미설정시, recvfrom을 호출하면 소켓에 아직 수신된 데이터그램이 없으면 스레드가 블로킹되어 수신 대기
	//(2) 수신버퍼는 따로 사본을 저장하지 않음
	//(3) 수신버퍼 크기 이상은 오버플로방지를 위해 복사 안함. 나머지는 버려지므로 넉넉히 잡기
	//(4) 플래그는 데이터 수신 제어 비트 플래그. 0이면 충분하나 MSG_PEEK 등의 플래그가 존재
	//(5) 받아온 주소는 채우는 값이 아니라 초기화되는 값임. 어디에서 받아왔는지 확인하는 용도
	
	 //성공시 수신버퍼에 복사한 바이트 길이를 리턴, 이외 -1 리턴
	if (byteReadCount >= 0)
	{
		return byteReadCount;
	}
	else
	{
		int ErrorCode = SocketUtil::GetLastError();

		//논블로킹 모드라면 수신한 데이터가 없을 때 여기로 들어감
		if (ErrorCode == WSAEWOULDBLOCK)
		{
			return 0;
		}
		else if (ErrorCode == WSAECONNRESET)
		{
			//클라이언트가 닫혔을 때 발생가능한 에러
			LOG("Connection reset from %s", outFromAddress.ToString().c_str());
			return -WSAECONNRESET;
		}
		else
		{
			//에러코드를 음수로 리턴
			SocketUtil::ReportError("UDPSocket::ReceiveFrom");
			return -ErrorCode;
		}
	}
}

int UDPSocket::SetNonBlockingMode(bool InShouldBeNonBlocking)
{
	u_long arg = InShouldBeNonBlocking ? 1 : 0;//1: 논블로킹(0 이외의 숫자), 0 : 블로킹
	//소켓을 논블로킹 모드로 만들기
	int result = ioctlsocket(mSocket, FIONBIO, &arg);
	//인자는 (1) 논블로킹하고자 하는 소켓 (2)제어하려는 소켓 파라미터 (3)파라미터에 설정하려는 값. 0 이외면 논블로킹.

	if (result != SOCKET_ERROR)
	{
		return NO_ERROR;
	}

	SocketUtil::ReportError("UDPSocket::SetNonBlockingMode");
	return SocketUtil::GetLastError();
}
