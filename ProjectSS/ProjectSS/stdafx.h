#pragma once
#pragma comment(lib, "ws2_32")
#define WIN32_LEAN_AND_MEAN

#include "SDL.h"

#include <stdio.h>
#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <string>
#include <algorithm>
#include <vector>
#include <memory>

#include "SocketAddress.h"
#include "SocketAddressFactory.h"
#include "SocketUtil.h"
#include "StringUtils.h"
#include "ByteSwapper.h"

#include "GamePlayUtils.h"

#include "MemberVariable.h"
#include "UDPSocket.h"
#include "TCPSocket.h"
#include "GameObjectSubClasses.h"

#include "MemoryBitStream.h"
#include "OutputMemoryStream.h"
#include "InputMemoryStream.h"
#include "OutputMemoryBitStream.h"
#include "InputMemoryBitStream.h"

#include "LinkingContext.h"
#include "GameInfo.h"
#include "MathUtils.h"
#include "ObjectCreationRegistry.h"

#include "ReplicationManager.h"
#include "ReplicationHeader.h"
#include "TimeUtil.h"

#include "NetworkManager.h"
#include "InputState.h"
#include "Move.h"
#include "MoveList.h"
#include "InputManager.h"