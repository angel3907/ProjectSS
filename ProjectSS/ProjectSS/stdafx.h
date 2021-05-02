#pragma once
#pragma comment(lib, "ws2_32")
#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <string>

#include "SocketAddress.h"
#include "SocketAddressFactory.h"
#include "SocketUtil.h"
#include "StringUtils.h"

#include "UDPSocket.h"
#include "TCPSocket.h"

#include "Player.h"