#include "stdafx.h"
#include <tchar.h>

void StringUtils::Log(const char* inFormat, ...)
{
	//가변 인자 함수 만드는 법
	//1. 함수 원형에 앞에 1개 이상의 인자, 그 뒤에 ...으로 인자 값을 사용한다.
	
	//2. va_list 변수를 정의 (char* 형식)
	va_list args;

	//3. va_start 함수로 va_list 변수에 가변인자의 정보를 담는다.
	//가변인자의 각 값을 메모리 공간에 저장하고, 메모리공간의 포인터 값을 va_list에 담아줌.
	va_start(args, inFormat);

	//4. _vscprintf 함수로 필요한 버퍼 크기를 얻고, 해당 크기만큼 메모리를 할당함.
	//서식이 적용된 문자열에 가변인자 정보를 적용했을 때 만들어지는 문자열의 길이를 반환함.
	int len = _vscprintf(inFormat, args) + 1; //for \0
	char* pBuf = new char[len];

	//5. _vsprintf_s 함수를 이용해 준비한 메모리에 문자열을 만듦.
	//할당해둔 메모리 주소에 해당 문자열을 담는다.
	if (pBuf)
	{
		_vsnprintf_s(pBuf, len, len, inFormat, args);
		printf("%s\n", pBuf);
		delete[] pBuf;
	}

	//6. va_end로 va_list에 할당된 메모리 공간을 해제.
	va_end(args);
}