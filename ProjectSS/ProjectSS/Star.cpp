#include "stdafx.h"

void StarStatus::Write(OutputMemoryBitStream& InStream) const
{
	InStream.Write(Value);
	InStream.Write(bHidden);
}

void StarStatus::Read(InputMemoryBitStream& InStream)
{
	InStream.Read(Value);
	InStream.Read(bHidden);
	SetValue(Value);
}

void StarStatus::WriteChanged(OutputMemoryBitStream& InStream) const
{
	//��Ʈ �ʵ���� �о�� ���� �о�� �� �Ӽ��� �Ǵ� ����
	InStream.Write(mProperties, GetRequiredBits(static_cast<uint32_t>(SSP_MAX)));

	//������ �Ӽ��� �����
	if ((mProperties & SSP_Value) != 0)
		InStream.Write(Value);

	if ((mProperties & SSP_Value) != 0)
		InStream.Write(bHidden);
}

void StarStatus::ReadChanged(InputMemoryBitStream& InStream)
{
	uint32_t WrittenProperties = 0;
	InStream.Read(WrittenProperties, GetRequiredBits(static_cast<uint32_t>(SSP_MAX)));

	if ((WrittenProperties & SSP_Value) != 0)
		InStream.Read(Value);

	if ((WrittenProperties & SSP_Value) != 0)
		InStream.Read(bHidden);
}

DataType* StarStatus::sDataType = nullptr;

void Star::Write(OutputMemoryBitStream& InStream) const
{
	GameObject::Write(InStream);
	mStarStatus.Write(InStream);
}

void Star::Read(InputMemoryBitStream& InStream)
{
	GameObject::Read(InStream);
	mStarStatus.Read(InStream);
}

void Star::WriteChanged(OutputMemoryBitStream& InStream) const
{
	GameObject::WriteChanged(InStream);
	mStarStatus.WriteChanged(InStream);
}

void Star::ReadChanged(InputMemoryBitStream& InStream)
{
	GameObject::ReadChanged(InStream);
	mStarStatus.ReadChanged(InStream);
}
