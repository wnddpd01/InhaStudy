#include "MyPacket.h"

MyPacket::MyPacket()
{
	clear();
}


MyPacket::~MyPacket()
{
}

void MyPacket::clear()
{
	packetData.packetNumber = PACKET_NULL;
	memset(packetData.msg, NULL, MAX_PACKET_MSG_SIZE);
}

MyPacketData * MyPacket::getPacketData()
{
	return &packetData;
}

void MyPacket::setPacketNumber(unsigned int pn)
{
	this->packetData.packetNumber = pn;
}

unsigned int MyPacket::getPacketNumber()
{
	return this->packetData.packetNumber;
}

void MyPacket::setMsg(char * msg)
{
	for (size_t i = 0; i < MAX_PACKET_MSG_SIZE && msg[i] != '\0' ; i++)
	{
		this->packetData.msg[i] = msg[i];
	}
}

void MyPacket::setMsg(POINTS * pt)
{
	itoa(pt->x, this->packetData.msg, 10);
	size_t ptYIdx = strlen(packetData.msg);
	this->packetData.msg[ptYIdx] = ',';
	itoa(pt->y, &(this->packetData.msg[ptYIdx + 1]), 10);
	this->packetData.msg[strlen(packetData.msg)] = ',';
	this->packetData.msg[strlen(packetData.msg)] = (char)StoneType::Empty;
}

void MyPacket::setMsg(Stone & stone)
{
	itoa(stone.pos.x, this->packetData.msg, 10);
	size_t ptYIdx = strlen(packetData.msg);
	this->packetData.msg[ptYIdx] = ',';
	itoa(stone.pos.y, &(this->packetData.msg[ptYIdx + 1]), 10);
	this->packetData.msg[strlen(packetData.msg)] = ',';
	this->packetData.msg[strlen(packetData.msg)] = (char)stone.stoneType;
}

const char * MyPacket::getMsg()
{
	return this->packetData.msg;
}

Stone MyPacket::getStone()
{
	Stone stone;
	size_t commaIdx = 0;
	size_t msgLen = strlen(packetData.msg);
	for (size_t i = 0; i < msgLen; i++)
	{
		if (packetData.msg[i] == ',')
		{
			commaIdx = i;
			break;
		}
	}
	stone.stoneType = (StoneType)packetData.msg[msgLen - 1];
	packetData.msg[msgLen - 2] = '\0';
	stone.pos.y = atoi(&(packetData.msg[commaIdx + 1]));
	packetData.msg[commaIdx] = '\0';
	stone.pos.x = atoi(packetData.msg);
	packetData.msg[msgLen - 2] = ',';
	packetData.msg[commaIdx] = ',';
	return stone;
}

StoneType MyPacket::getStoneType()
{
	return (StoneType)packetData.msg[0];
}

