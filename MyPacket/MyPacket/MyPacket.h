#pragma once

#include <Windows.h>

#ifndef MYPACKET
#ifdef DLL_EXPORTS
#define MYPACKET __declspec(dllexport)
#else
#define MYPACKET __declspec(dllimport)
#endif
#endif

enum class StoneType : char
{
	Empty = 0, Black, White
};

#define PACKET_NULL		0
#define PACKET_CHAT		1
#define PACKET_MAP		2
#define PACKET_POINT	3
#define PACKET_STONE	4
#define PACKET_ERROR	999
#define MAX_PACKET_MSG_SIZE 512

#define BLACK_STONE 0
#define WHITE_STONE 1

typedef MYPACKET struct MyPacketData
{
	unsigned int packetNumber;
	char msg[MAX_PACKET_MSG_SIZE];
}MyPacketData;


class MYPACKET MyPacket
{
	MyPacketData packetData;
public:
	MyPacket();
	~MyPacket();

	void clear();

	MyPacketData* getPacketData();

	void setPacketNumber(unsigned int pn);
	unsigned int getPacketNumber();

	void setMsg(char *msg);
	void setMsg(POINTS *pt);
	const char *getMsg();
	POINTS getPOINTS();
	StoneType getStoneType();
};

