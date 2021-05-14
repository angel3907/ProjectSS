#include "stdafx.h"

void Player::ProcessInput(float InDeltaTime, const InputState& InCurrentState)
{
	//��ǲ ó��
	//�̵���, ������ ó��
	float NewPosX = GetPos().PosX + InCurrentState.GetDesiredHorizontalDelta() * InDeltaTime * Speed;
	float NewPosY = GetPos().PosY - InCurrentState.GetDesiredVerticallDelta() * InDeltaTime * Speed;

	SetPos(Vector2(NewPosX, NewPosY));
	mIsAttacking = InCurrentState.IsAttacking();
}

void Player::SimulateMovement(float InDeltaTime)
{
	//������ �ӵ� ����, �׿� ���� ��ġ ����, �ݸ��� ó���� ����
	//���� �Ұ� ������ ���д�
}

void Player::Write(OutputMemoryStream& InStream) const
{
	//Ŭ���� ��ü���� Write�� �ϰ� �ϸ�, ���� private �ʵ忡�� ������ ���� �����ϹǷ�
	//����ȭ �ڵ带 ������ ���� �ڵ�� �и� ����.
	InStream.Write(StarCount);
}

void Player::Read(InputMemoryStream& InStream)
{
	InStream.Read(StarCount);
}

void Player::WriteChanged(OutputMemoryBitStream& InStream) const
{
	GameObject::WriteChanged(InStream);
	
	//��Ʈ �ʵ���� �о�� ���� �о�� �� �Ӽ��� �Ǵ� ����
	InStream.Write(mProperties, GetRequiredBits(static_cast<uint32_t>(PLR_MAX)));

	//������ �Ӽ��� �����
	if ((mProperties & PLR_StarCount) != 0)
		InStream.Write(StarCount);

	if ((mProperties & PLR_TestValue) != 0)
		InStream.Write(TestValue);

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

	if ((WrittenProperties & PLR_TestValue) != 0)
		InStream.Read(TestValue);

	if ((WrittenProperties & PLR_Name) != 0)
		InStream.Read(Name);
}

void Player::Write(OutputMemoryBitStream& InStream) const
{
 	InStream.Write(TestValue);
 	InStream.Write(StarCount);
	InStream.Write(Name);
	InStream.WritePosF(Pos);

	//��� �迭 ����
	//NickName�� �� ���� ���ڿ��� ��.
// 	uint8_t NameLength = static_cast<uint8_t>(strlen(Nickname));
// 	InStream.Write(NameLength);
// 	InStream.WriteBytes(Nickname, NameLength);
}

void Player::Read(InputMemoryBitStream& InStream)
{
 	InStream.Read(TestValue);
 	InStream.Read(StarCount);
	InStream.Read(Name);
	InStream.ReadPosF(Pos);

	//NickName�� �� ���� ���ڿ��� ��.
	//uint8_t NameLength;
	//InStream.Read(NameLength);
	//InStream.ReadBytes(Nickname, NameLength);

}
