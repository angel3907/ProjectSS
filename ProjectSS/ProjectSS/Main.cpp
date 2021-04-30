#include "stdafx.h"

int main()
{
	//1. 소켓 라이브러리 활성화 (명시적으로 초기화&마무리해줘야 하며, 라이브러리 버전도 설정해야 함)
	WSADATA WsaData;
	WSAStartup(MAKEWORD(2, 2), &WsaData); //주버전번호 & 부버전번호, WSAStartup()함수가 활성화된 라이브러리에 대한 정보로 값을 채워줌.
										  //성공시 0 리턴, 실패시 에러코드 리턴

	SOCKET UdpSocket = socket(AF_INET, SOCK_DGRAM, 0); //IPV4, 데이터그램 단위(for UDP), 소켓 종류에 맞춰 프로토콜 자동 지정(0)
	SOCKET TcpSocket = socket(AF_INET, SOCK_STREAM, 0); //IPV4, 세그먼트 단위(for TCP), 소켓 종류에 맞춰 프로토콜 자동 지정(0) 

	//IPV4 패킷용 주소 구조체 - 예시 1
	sockaddr_in SockAddr{};
	SockAddr.sin_family = AF_INET; //주소 종류를 나타내는 상수값. 소켓에 사용된 것과 같아야 함
	
	//소켓 주소 구조체에서 여러 바이트로 된 숫자를 호스트 순서가 아니라 네트워크 순서 체계로 변환해야 한다.
	//변환하는 함수는 htons() htonl() (host to network short(16비트), host to network long(32비트))
	SockAddr.sin_port = htons(7000); //포트값, 2바이트 (예약된 포트 제외한 임의값으로 넣어줬음)
	SockAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//4바이트 ipv4 주소. 유니온으로 감싸둔 구조체. (iNADDR_ANY : 현재 컴퓨터의 주소)
	//마지막으로 sin_zero 멤버가 있는데 이는 패딩값. 0으로 채울 것

	//만일 패킷을 수신하는 경우 소켓 라이브러리가 sockaddr_in 구조체 내용을 채워주는 걍우도 있음.
	//이때 sockaddr_in의 각 필드는 네트워크 바이트 순서로 채워져 있으니 이를 올바른 값으로 사용하려면
	//ntohs(), ntohl() (network to host short(16비트), network to host long(32비트))을 호출해 네트워크 -> 호스트 바이트 순서로 변환.

	//예시 2
	sockaddr_in SockAddr2{};
	SockAddr2.sin_family = AF_INET; //주소 종류를 나타내는 상수값. 소켓에 사용된 것과 같아야 함
	SockAddr.sin_port = htons(80); //포트값, 2바이트
	SockAddr.sin_addr.S_un.S_un_b.s_b1 = 65; //4바이트 ipv4 주소, 바이트 단위로 넣으니 변환해줄 필요가 없음.
	SockAddr.sin_addr.S_un.S_un_b.s_b2 = 254;
	SockAddr.sin_addr.S_un.S_un_b.s_b3 = 248;
	SockAddr.sin_addr.S_un.S_un_b.s_b4 = 180;


	//소켓 바인딩
	//-> 어떤 소켓이 특정 주소와 포트번호를 쓰겠다고 운영체제에게 알려주는 절차.
	//바인딩의 의미 2가지
	//1) os가 이 주소,포트를 목적지로 발신된 패킷 수신시 os는 이 소켓에게 넘겨줌
	//2) bind에서 지정한 주소,포트를 이 소켓이 발신하는 패킷의 네트워크/전송 계층 헤더 발신 주소, 포트로 씀.

	//일반적으로 하나의 주소, 포트 당 한 소켓만 바인딩 가능. 이미 바인드된 주소로는 바인드에러남.
	//실패시 미사용 포트 찾을 때까지 바인딩 시도해야 하는데, 이때 포트번호 0으로 해주면 라이브러리가 자동으로 미사용중인 포트하나를 골라 바인딩함
	//데이터 송수신을 위해선 반드시 바인딩이 필요. 만약 바인딩 안 된 소켓으로 데이터를 보내려 하면 라이브러리가 자동으로 남은 포트에 소켓을 바인딩함.
	//그래서 클라는 냅둬도 되는데, 서버같이 공표된 주소와 포트로 패킷을 받아야 한다면 바인딩이 필요

	sockaddr_in SockAddr3{};
	SockAddr3.sin_family = AF_INET;
	SockAddr3.sin_port = htons(0);
	//호스트에는 여러개의 NIC가 있을 수 있고 각각 IP가 다름. INADDR_ANY로 해주면 모든 NIC의 IP주소의 해당 포트에 몽땅 바인딩하게 됨
	SockAddr3.sin_addr.S_un.S_addr = htonl(INADDR_ANY); 
 
	//bind() 함수의 인자는 바인딩할 소켓 / 소켓 주소 구조체 / 소켓 주소 구조체 사이즈
	int BindErrorCode = bind(TcpSocket, reinterpret_cast<sockaddr*>(&SockAddr3), sizeof(sockaddr));

	if (BindErrorCode != 0) //리턴값이 0이면 성공 -1이면 실패.
	{
		printf("Bind Fail!");
	}

	shutdown(TcpSocket, SD_SEND); //송신 중단
	shutdown(TcpSocket, SD_RECEIVE); //수신 중단
	shutdown(TcpSocket, SD_BOTH); //송수신 중단 ->송수신중인 모든 데이터를 송수신 완료후 FIN 패킷을 보내서 연결을 안전하게 닫도록 함.

	closesocket(UdpSocket); //소켓 닫기 (사용을 마치면 반드시 닫는다.)
	closesocket(TcpSocket); //소켓을 닫으면 관련 리소스를 운영체제에 반납

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