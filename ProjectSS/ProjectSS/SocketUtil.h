#pragma once
#include "UDPSocket.h"
#include "TCPSocket.h"
#include <vector>
enum SocketAddressFamily 
{
	INET = AF_INET,
	INET6 = AF_INET6
};

class SocketUtil
{
public:
	static UDPSocketPtr CreateUDPSocket(SocketAddressFamily inFamily);
	static TCPSocketPtr CreateTCPSocket(SocketAddressFamily inFamily);

	static fd_set* FillSetFromVector(fd_set& OutSet, const vector<TCPSocketPtr>* InSockets);
	static void FillVectorFromSet(vector<TCPSocketPtr>* OutSockets, const vector<TCPSocketPtr>* InSockets, const fd_set& InSet);

	static int Select(const vector<TCPSocketPtr>* InReadSet, vector<TCPSocketPtr>* OutReadSet, 
			   const vector<TCPSocketPtr>* InWriteSet, vector<TCPSocketPtr>* OutWriteSet,
			   const vector<TCPSocketPtr>* InExceptSet, vector<TCPSocketPtr>* OutExceptSet);

	static void	ReportError(const char* inOperationDesc);
	static int GetLastError();
};