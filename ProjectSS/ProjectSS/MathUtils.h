#pragma once
#include <stdint.h>
#include <cstdlib>

struct Vector2
{
	float PosX = 0;
	float PosY = 0;

	Vector2(float PosX = 0, float PosY = 0) : PosX(PosX), PosY(PosY) {}
	bool IsEqual(Vector2 InVector) 
	{
		return (abs(InVector.PosX - PosX) < 0.001f && abs(InVector.PosY - PosY) < 0.001f);
	}

	Vector2 operator+(const Vector2 &InVector)
	{
		Vector2 V(PosX + InVector.PosX, PosY + InVector.PosY);
		return V;
	}

	Vector2 operator-(const Vector2& InVector)
	{
		Vector2 V(PosX - InVector.PosX, PosY - InVector.PosY);
		return V;
	}

	Vector2 operator*(const float& InValue)
	{
		Vector2 V(PosX * InValue, PosY* InValue);
		return V;
	}

	Vector2 operator*(const int& InValue)
	{
		Vector2 V(PosX * InValue, PosY * InValue);
		return V;
	}

	Vector2 operator/(const float& InValue) const
	{
		Vector2 V(PosX / InValue, PosY / InValue);
		return V;
	}

	Vector2 operator/(const int& InValue) const
	{
		Vector2 V(PosX / InValue, PosY / InValue);
		return V;
	}

	static float GetLength(const Vector2& InVector)
	{
		return sqrtf(InVector.PosX * InVector.PosX + InVector.PosY * InVector.PosY);
	}

	static Vector2 GetNormalized(const Vector2& InVector)
	{
		return InVector / GetLength(InVector);
	}

};

struct Quaternion
{
	float X;
	float Y;
	float Z;
	float W;
};

//������ ����ȭ�� ������ ��Ʈ ���� ������ Ÿ�ӿ� ������
constexpr int32_t GetRequiredBits(int32_t Value, int Bits = 0)
{
	return Value ? GetRequiredBits(Value >> 1, Bits + 1) : Bits;
}

inline bool IsCircleCollided(Vector2 PlayerPosA, Vector2 PlayerPosB, float CollisionRadiusA, float CollisionRadiusB)
{
	float Length = Vector2::GetLength(PlayerPosA - PlayerPosB);
	return (Length < (CollisionRadiusA + CollisionRadiusB));
}

inline int GetRandomInt(int StarValue, int EndValue)
{
	int Res = StarValue + (rand() % (EndValue - StarValue));
	return Res;
}