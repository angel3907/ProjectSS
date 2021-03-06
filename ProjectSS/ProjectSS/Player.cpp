#include "stdafx.h"
#include "SDLRenderer.h"

void Player::ProcessInput(float InDeltaTime, const InputState& InCurrentState)
{
	//인풋 처리
	//이동값, 공격중 처리
	
	if (!bAttacked) //공격당하는 중에는 이동 입력을 처리하지 않음
	{
		float NewPosX = GetPos().PosX + InCurrentState.GetDesiredHorizontalDelta() * InDeltaTime * Speed;
		float NewPosY = GetPos().PosY - InCurrentState.GetDesiredVerticallDelta() * InDeltaTime * Speed;

		Vector2 NewPos(NewPosX, NewPosY);
		ApplyPosLimit(NewPos);
		SetPos(NewPos);
	}

	mIsAttacking = InCurrentState.IsAttacking();
}

void Player::SimulateMovement(float InDeltaTime)
{
	//기존은 속도 설정, 그에 따른 위치 설정, 콜리전 처리를 해줌
	//나는 할게 없으니 냅둔다
}

bool Player::ApplyPosLimit(Vector2& InOutPos)
{
	bool IsPosLimited = false;

	//이동 최대값 조정
	//X
	if (InOutPos.PosX > WORLD_WIDTH - PlayerRadius)
	{
		InOutPos.PosX = WORLD_WIDTH - PlayerRadius;
		IsPosLimited = true;
	}
	else if (InOutPos.PosX < PlayerRadius)
	{
		InOutPos.PosX = PlayerRadius;
		IsPosLimited = true;
	}

	//Y
	if (InOutPos.PosY > WORLD_HEIGHT - PlayerRadius)
	{
		InOutPos.PosY = WORLD_HEIGHT - PlayerRadius;
		IsPosLimited = true;
	}
	else if (InOutPos.PosY < PlayerRadius)
	{
		InOutPos.PosY = PlayerRadius;
		IsPosLimited = true;
	}
	
	return IsPosLimited;
}

void Player::Write(OutputMemoryStream& InStream) const
{
	//클래스 자체에서 Write를 하게 하면, 내부 private 필드에서 스스로 접근 가능하므로
	//직렬화 코드를 데이터 전송 코드와 분리 가능.
	InStream.Write(StarCount);
}

void Player::Read(InputMemoryStream& InStream)
{
	InStream.Read(StarCount);
}

void Player::WriteChanged(OutputMemoryBitStream& InStream) const
{
	GameObject::WriteChanged(InStream);
	
	//비트 필드부터 읽어야 실제 읽어야 할 속성을 판단 가능
	InStream.Write(mProperties, GetRequiredBits(static_cast<uint32_t>(PLR_MAX)));

	//지정된 속성만 기록함
	if ((mProperties & PLR_StarCount) != 0)
		InStream.Write(StarCount);

	if ((mProperties & PLR_Name) != 0)
		InStream.Write(Name);
}

void Player::ReadChanged(InputMemoryBitStream& InStream)
{
	GameObject::ReadChanged(InStream);
	uint32_t WrittenProperties = 0;
	InStream.Read(WrittenProperties, GetRequiredBits(static_cast<uint32_t>(PLR_MAX)));

	if ((WrittenProperties & PLR_StarCount) != 0)
		InStream.Read(StarCount);

	if ((WrittenProperties & PLR_Name) != 0)
		InStream.Read(Name);
}

void Player::Write(OutputMemoryBitStream& InStream) const
{	
	InStream.Write(mPlayerId);
 	InStream.Write(StarCount);
	InStream.Write(Name);
	InStream.Write(mPlayerColor);
	InStream.WritePosF(Pos);
	InStream.Write(bAttackEffectOn);

	//희소 배열 압축
	//NickName이 널 종료 문자열일 때.
// 	uint8_t NameLength = static_cast<uint8_t>(strlen(Nickname));
// 	InStream.Write(NameLength);
// 	InStream.WriteBytes(Nickname, NameLength);
}

void Player::Read(InputMemoryBitStream& InStream)
{
	InStream.Read(mPlayerId);
 	InStream.Read(StarCount);
	InStream.Read(Name);
	InStream.Read(mPlayerColor);
	InStream.ReadPosF(Pos);
	InStream.Read(bAttackEffectOn);

	//NickName이 널 종료 문자열일 때.
	//uint8_t NameLength;
	//InStream.Read(NameLength);
	//InStream.ReadBytes(Nickname, NameLength);

}
