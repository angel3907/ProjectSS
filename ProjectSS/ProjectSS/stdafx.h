#pragma once
#pragma comment(lib, "ws2_32") //WinSock2.h�� ������ ����� ����ϱ� ���� ��ũ���� ���̺귯��
#define WIN32_LEAN_AND_MEAN //Winsock.h�� Winsock2�� �̸� ��ħ�� ����

#include <stdio.h>
#include <WinSock2.h> //Socket ���̺귯��
#include <Windows.h>
#include <WS2tcpip.h>
#include <string>


//���� ���� ���̺귯��
#include "SocketAddress.h"
#include "SocketAddressFactory.h"
#include "SocketUtil.h"
#include "UDPSocket.h"
#include "TCPSocket.h"
#include "StringUtils.h"