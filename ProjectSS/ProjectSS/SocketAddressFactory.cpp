#include "stdafx.h"

SocketAddressPtr SocketAddressFactory::CreateIPv4FromIPString(const std::string& InString)
{
	/*���ڿ��� �� IP�� sockaddr�� ��ȯ�ϴ� �Լ�
		int InetPton(int af, const PCTSTR src, void* dst);
		af : �ּ��йи�
		src : ���ڿ��� �� IP�ּ� (������ ���ڿ�, ��ħǥ ����)
		dst : ��ȯ�� sin_addr �ּ� �ʵ��� ������
		������ 1����, �ؼ��Ұ��� 0����, �ý��� ������ -1����
	*/

	auto Pos = InString.find_last_of(':');
	std::string Ip, Port;

	if (Pos != std::string::npos)
	{
		Ip = InString.substr(0, Pos);
		Port = InString.substr(Pos + 1);
	}
	else
	{
		Ip = InString;
		Port = "0";
	}

	sockaddr_in Addr{};
	Addr.sin_family = AF_INET;
	Addr.sin_port = htons(stoi(Port));
	
	int ErrorCode = inet_pton(AF_INET, Ip.c_str(), &Addr.sin_addr);

	if (ErrorCode == SOCKET_ERROR)
	{
		SocketUtil::ReportError("SocketAddressFactory::CreateIPv4FromIPString");
		return nullptr;
	}
	else if (ErrorCode == 0)
	{
		printf("Cannot Translate Ip Address at SocketAddressFactory::CreateIPv4FromIPString");
	}

	sockaddr AddrToRet{};
	memcpy(&AddrToRet, &Addr, sizeof(Addr));

	//������ �����ּ� ��ü�� ���� ����
	SocketAddressPtr toRet = std::make_shared<SocketAddress>(AddrToRet);
	return toRet;
}

SocketAddressPtr SocketAddressFactory::CreateIPv4FromDomainString(const std::string& inString)
{
	/*������ �̸��� IP�ּҷ� ��ȯ�ϴ� �Լ�
		int getaddrinfo(const char* hostname, const char* servname, const addrinfo* hints, addrinfo** res);
		hostname : ������ �̸�
		servname : ��Ʈ��ȣ �Ǵ� ���� �̸�
		hints : ȣ���ڰ� � ������ �ް������ �����ص� addrinfo ����ü ������
		res : DNS ���� ��ȸ ���
	*/

	//�̸����� �ݷ��� ã�� ��Ʈ��ȣ �и�
	auto pos = inString.find_last_of(':');
	std::string host, service;

	if (pos != std::string::npos) //��Ʈ��ȣ�� �ִٸ�
	{
		host = inString.substr(0, pos); //�߶� ���ڿ� ���� ��ġ, ����
		service = inString.substr(pos + 1); //�߶� ���ڿ� ���� ��ġ (���� ������)
	}
	else //��Ʈ��ȣ�� ���ٸ� ����Ʈ
	{
		host = inString;
		service = "0";
	}

	addrinfo hint{};
	hint.ai_family = AF_INET; //�ش� �ּ� �йи��͸� ���� �� ����

	addrinfo* result = nullptr;
	int error = getaddrinfo(host.c_str(), service.c_str(), &hint, &result); //*WS2tcpip ����� ����
	addrinfo* initResult = result;

	if (error != 0 && result != nullptr) //������ �߻������� ����� �ִٸ�
	{
		freeaddrinfo(initResult); //�޸� ���� (getaddrinfo()�� addrinfo ����ü ��ȯ�� ��ü������ �޸𸮸� �Ҵ��ϹǷ�)
		return nullptr;
	}

	while (!result->ai_addr && result->ai_next) //���� �׸��� �ּҰ��� null�̰�, ���� �׸��� �ִٸ�
	{
		result = result->ai_next; //���� addrinfo�� Ȯ����
	}

	if (!result->ai_addr) //������ �׸���� �����Ƿ� result�� ������ �׸��̸�, �� �׸��� �ּҰ� null�̶��
	{
		freeaddrinfo(initResult); //���� �޸� ����
		return nullptr;
	}

	//������ �����ּ� ��ü�� ���� ����
	auto toRet = std::make_shared<SocketAddress>(*result->ai_addr);

	freeaddrinfo(initResult); //�޸� ����
	return toRet;

}

