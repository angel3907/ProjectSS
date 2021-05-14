#pragma once

struct Vector2
{
	float PosX = 0;
	float PosY = 0;

	Vector2(float PosX = 0, float PosY = 0) : PosX(PosX), PosY(PosY) {}
	bool IsEqual(Vector2 InVector) 
	{
		return (abs(InVector.PosX - PosX) < 0.001f && abs(InVector.PosY - PosY) < 0.001f);
	}
};

struct Quaternion
{
	FLOAT X;
	FLOAT Y;
	FLOAT Z;
	FLOAT W;
};

//열거자 직렬화시 최적의 비트 수를 컴파일 타임에 결정함
constexpr int32_t GetRequiredBits(int32_t Value, int Bits = 0)
{
	return Value ? GetRequiredBits(Value >> 1, Bits + 1) : Bits;
}