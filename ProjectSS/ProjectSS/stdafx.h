#pragma once
#pragma comment(lib, "ws2_32")
#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <string>
#include <algorithm>
#include <vector>

#include "SocketAddress.h"
#include "SocketAddressFactory.h"
#include "SocketUtil.h"
#include "StringUtils.h"
#include "ByteSwapper.h"


#include "MemberVariable.h"
#include "UDPSocket.h"
#include "TCPSocket.h"
#include "GameObject.h"
#include "Player.h"
#include "Star.h"

#include "MemoryBitStream.h"

#include "OutputMemoryStream.h"
#include "InputMemoryStream.h"
#include "OutputMemoryBitStream.h"
#include "InputMemoryBitStream.h"

#include "LinkingContext.h"
#include "GameInfo.h"
#include "MathUtils.h"
