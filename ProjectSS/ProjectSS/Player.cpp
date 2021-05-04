#include "stdafx.h"

void Player::NaivelySendPlayer(int InSocket, const Player* InPlayer)
{
	send(InSocket, reinterpret_cast<const char*>(InPlayer), sizeof(*InPlayer) , 0);
}

void Player::NaivelyReceivePlayer(int InSocket, Player* OutPlayer)
{
	recv(InSocket, reinterpret_cast<char*>(OutPlayer), sizeof(*OutPlayer), 0);
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