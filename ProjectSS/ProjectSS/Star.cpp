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
}

void StarStatus::WriteChanged(OutputMemoryBitStream& InStream, uint32_t InProperties) const
{
	//��Ʈ �ʵ���� �о�� ���� �о�� �� �Ӽ��� �Ǵ� ����
	InStream.Write(InProperties, GetRequiredBits(static_cast<uint8_t>(SSP_MAX)));

	//������ �Ӽ��� �����
	if ((InProperties & SSP_Value) != 0)
		InStream.Write(Value);

	if ((InProperties & SSP_Value) != 0)
		InStream.Write(bHidden);
}

void StarStatus::ReadChanged(InputMemoryBitStream& InStream)
{
	uint32_t WrittenProperties = 0;
	InStream.Read(WrittenProperties, GetRequiredBits(static_cast<uint8_t>(SSP_MAX)));

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