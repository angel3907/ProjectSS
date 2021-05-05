#pragma once

struct Vector2
{
	float PosX = 0;
	float PosY = 0;

	Vector2(float PosX = 0, float PosY = 0) : PosX(PosX), PosY(PosY) {}
};

struct Quaternion
{
	FLOAT X;
	FLOAT Y;
	FLOAT Z;
	FLOAT W;
};

//������ ����ȭ�� ������ ��Ʈ ���� ������ Ÿ�ӿ� ������
constexpr int32_t GetRequiredBits(int32_t Value, int Bits = 0)
{
	return Value ? GetRequiredBits(Value >> 1, Bits + 1) : Bits;
}