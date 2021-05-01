#include "stdafx.h"
#include <tchar.h>

void StringUtils::Log(const char* inFormat, ...)
{
	//���� ���� �Լ� ����� ��
	//1. �Լ� ������ �տ� 1�� �̻��� ����, �� �ڿ� ...���� ���� ���� ����Ѵ�.
	
	//2. va_list ������ ���� (char* ����)
	va_list args;

	//3. va_start �Լ��� va_list ������ ���������� ������ ��´�.
	//���������� �� ���� �޸� ������ �����ϰ�, �޸𸮰����� ������ ���� va_list�� �����.
	va_start(args, inFormat);

	//4. _vscprintf �Լ��� �ʿ��� ���� ũ�⸦ ���, �ش� ũ�⸸ŭ �޸𸮸� �Ҵ���.
	//������ ����� ���ڿ��� �������� ������ �������� �� ��������� ���ڿ��� ���̸� ��ȯ��.
	int len = _vscprintf(inFormat, args) + 1; //for \0
	char* pBuf = new char[len];

	//5. _vsprintf_s �Լ��� �̿��� �غ��� �޸𸮿� ���ڿ��� ����.
	//�Ҵ��ص� �޸� �ּҿ� �ش� ���ڿ��� ��´�.
	if (pBuf)
	{
		_vsnprintf_s(pBuf, len, len, inFormat, args);
		printf("%s\n", pBuf);
		delete[] pBuf;
	}

	//6. va_end�� va_list�� �Ҵ�� �޸� ������ ����.
	va_end(args);
}