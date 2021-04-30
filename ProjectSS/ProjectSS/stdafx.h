#pragma once
#pragma comment(lib, "ws2_32") //WinSock2.h에 선언한 기능을 사용하기 위해 링크해준 라이브러리
#define WIN32_LEAN_AND_MEAN //Winsock.h와 Winsock2의 이름 겹침을 막음

#include <stdio.h>
#include <WinSock2.h> //Socket 라이브러리
#include <Windows.h>
#include <WS2tcpip.h>
#include <string>


//게임 관련 라이브러리
#include "SocketAddress.h"
#include "SocketAddressFactory.h"
#include "SocketUtil.h"
#include "UDPSocket.h"