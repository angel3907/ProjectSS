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
	//클래스 자체에서 Write를 하게 하면, 내부 private 필드에서 스스로 접근 가능하므로
	//직렬화 코드를 데이터 전송 코드와 분리 가능.
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

	//희소 배열 압축
	//NickName이 널 종료 문자열일 때.
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

	//NickName이 널 종료 문자열일 때.
	//uint8_t NameLength;
	//InStream.Read(NameLength);
	//InStream.ReadBytes(Nickname, NameLength);

}
