#pragma once
namespace StringUtils
{
	void	Log(const char* inFormat);
	void	Log(const char* inFormat, ...);
}

#define LOG( ... ) StringUtils::Log( __VA_ARGS__ );