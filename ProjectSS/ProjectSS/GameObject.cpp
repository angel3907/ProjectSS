#include "stdafx.h"

void GameObject::Write(OutputMemoryBitStream& InStream) const
{
	InStream.WritePosF(Pos);
}

void GameObject::Read(InputMemoryBitStream& InStream)
{
	InStream.ReadPosF(Pos);
}

void GameObject::WriteChanged(OutputMemoryBitStream& InStream) const
{
	//��Ʈ �ʵ���� �о�� ���� �о�� �� �Ӽ��� �Ǵ� ����
	InStream.Write(mProperties, GetRequiredBits(static_cast<uint32_t>(GOP_MAX)));

	//������ �Ӽ��� �����
	if ((mProperties & GOP_Pos) != 0)
		InStream.WritePosF(Pos);
}

void GameObject::ReadChanged(InputMemoryBitStream& InStream)
{
	uint32_t WrittenProperties = 0;
	InStream.Read(WrittenProperties, GetRequiredBits(static_cast<uint32_t>(GOP_MAX)));

	if ((WrittenProperties & GOP_Pos) != 0)
		InStream.ReadPosF(Pos);
}