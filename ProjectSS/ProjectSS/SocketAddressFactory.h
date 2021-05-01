#pragma once
#include "stdafx.h"

/*문자열로 된 IP를 sockaddr로 변환하는 함수
  int InetPton(int af, const PCTSTR src, void* dst);
  af : 주소패밀리
  src : 문자열로 된 IP주소 (널종료 문자열, 마침표 구분)
  dst : 변환된 sin_addr 주소 필드의 포인터
  성공시 1리턴, 해석불가시 0리턴, 시스템 에러시 -1리턴
*/

class SocketAddressFactory
{
public:
	static SocketAddressPtr CreateIPv4FromString(const string& inString) //인자는 호스트이름+포트번호
	{
		/*도메인 이름을 IP주소로 변환하는 함수
		  int getaddrinfo(const char* hostname, const chae* servname, const addrinfo* hints, addrinfo** res);
		  hostname : 도메인 이름
		  servname : 포트번호 또는 서비스 이름
		  hints : 호출자가 어떤 정보를 받고싶은지 기재해둔 addrinfo 구조체 포인터
		  res : DNS 서버 조회 결과
		*/

		//이름에서 콜론을 찾아 포트번호 분리
		auto pos = inString.find_last_of(':');
		string host, service;

		if (pos != string::npos) //포트번호가 있다면
		{
			host = inString.substr(0, pos); //잘라낼 문자열 시작 위치, 길이
			service = inString.substr(pos+1); //잘라낼 문자열 시작 위치 (부터 끝까지)
		}
		else //포트번호가 없다면 디폴트
		{
			host = inString;
			service = "0";
		}

		addrinfo hint{};
		hint.ai_family = AF_INET; //해당 주소 패밀리것만 받을 수 있음
		
		addrinfo* result = nullptr;
		int error = getaddrinfo(host.c_str(), service.c_str(), &hint, &result); //*WS2tcpip 헤더에 포함
		addrinfo* initResult = result;

		if (error != 0 && result != nullptr) //에러가 발생했으나 결과가 있다면
		{
			freeaddrinfo(initResult); //메모리 해제 (getaddrinfo()가 addrinfo 구조체 반환시 자체적으로 메모리를 할당하므로)
			return nullptr;
		}

		while (!result->ai_addr && result->ai_next) //현재 항목의 주소값이 null이고, 다음 항목이 있다면
		{
			result = result->ai_next; //다음 addrinfo를 확인함
		}

		if (!result->ai_addr) //마지막 항목까지 갔으므로 result는 마지막 항목이며, 이 항목의 주소가 null이라면
		{
			freeaddrinfo(initResult); //역시 메모리 해제
			return nullptr;
		}

		//리턴할 소켓주소 객체를 만들어서 리턴
		auto toRet = std::make_shared<SocketAddress>(*result->ai_addr);

		freeaddrinfo(initResult); //메모리 해제
		return toRet;
	}
};